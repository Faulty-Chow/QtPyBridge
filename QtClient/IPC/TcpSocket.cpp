//
// Created by 87431 on 2025/3/25.
//

#include "TcpSocket.h"
#include <QTcpSocket>

using namespace QPB;

TcpSocket::TcpSocket(QObject *parent) : m_socket(new QTcpSocket(this)) {
    QObject::connect(m_socket, &QTcpSocket::connected, this, &PythonIO::connected);
    QObject::connect(m_socket, &QTcpSocket::disconnected, this, &PythonIO::disconnected);
    QObject::connect(m_socket, &QTcpSocket::errorOccurred, this,
                     [this](auto error) {
                         qCritical() << "Socket error: " << m_socket->errorString();
                         emit errorOccurred(error);
                     });
}

bool TcpSocket::set(Attribute attribute, const QVariant &data) {
    switch (attribute) {
        case Host:
            m_host = data.toString();
            return true;
        case Port:
            m_port = data.toUInt();
            return true;
        default:
            return false;
    }
}

void TcpSocket::open() {
    m_socket->connectToHost(m_host, m_port);
}

bool TcpSocket::waitForConnected(int msecs) {
    return m_socket->waitForConnected(msecs);
}

bool TcpSocket::isOpen() {
    return m_socket->isOpen();
}

void TcpSocket::close() {
    m_socket->close();
}

bool TcpSocket::write(const DataFrame &frame) {
    QByteArray packet = frame.toBytes();
    return m_socket->write(packet) == packet.size();
}

void TcpSocket::onReadyRead() {
    m_buffer.append(m_socket->readAll());
    while (true) {
        if (m_buffer.size() < sizeof(DataFrame::Header)) {
            break;
        }

        QDataStream stream(&m_buffer, QIODevice::ReadOnly);
        stream.setByteOrder(QDataStream::BigEndian);

        DataFrame::Header header;
        stream >> header.magic >> header.version;
        if (header.magic != DataFrame::MAGIC || header.version != DataFrame::VERSION) {
            m_buffer.remove(0, 1);
            continue;
        }
        stream >> header.type >> header.length >> header.reserve;

        const quint32 totalPacketSize = sizeof(DataFrame::Header) + header.length;
        if (m_buffer.size() < totalPacketSize) {
            break;
        }

        QByteArray packet = m_buffer.left(totalPacketSize);
        m_buffer.remove(0, totalPacketSize);

        DataFrame frame(packet);
        emit frameReady(frame);
    }
}
