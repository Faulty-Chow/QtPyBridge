//
// Created by 87431 on 2025/3/25.
//

#include "DataFrame.h"
#include <QBuffer>
#include <QDebug>

using namespace QPB;

DataFrame::DataFrame(QUuid id) : m_id(id) {
}

DataFrame::DataFrame(const QByteArray &rawData) {
    QDataStream stream(rawData);
    stream.setByteOrder(QDataStream::BigEndian);
    stream.readRawData(reinterpret_cast<char *>(&m_header), sizeof(Header));
    if (stream.status() != QDataStream::Ok) {
        qWarning() << "Failed to read header!";
        return;
    }
    m_data = stream.device()->readAll();
    if (m_header.length != m_data.size()) {
        qWarning() << "Invalid payload length!";
        return;
    }
}

QByteArray DataFrame::toPacket() const {
    QByteArray byteArray;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream << MAGIC;
    stream << VERSION;
    stream << m_type;
    stream << m_id;
    stream << m_data.length();
    stream << m_data;
    return byteArray;
}

int DataFrame::fromPacket(QBuffer &buffer, DataFrame &frame) {
    if (!buffer.isOpen()) {
        if (!buffer.open(QIODevice::ReadOnly)) {
            qWarning() << "Failed to open QBuffer!";
            return -1;
        }
    }

    QDataStream stream(&buffer);
    stream.setByteOrder(QDataStream::BigEndian);

    quint16 magic;
    stream >> magic;
    if (magic != MAGIC) {
        qWarning() << "Invalid magic number!";
        return -1;
    }

    quint8 version;
    stream >> version;
    if (version != VERSION) {
        qWarning() << "Invalid version!";
        return -1;
    }

    stream >> frame.m_type;
    stream >> frame.m_id;

    qsizetype length;
    stream >> length;

    frame.m_data = buffer.read(length);
    if (frame.m_data.size() != length) {
        qWarning() << "Failed to read payload!";
        return -1;
    }

    return buffer.pos();
}


quint8 DataFrame::type() const {
    return m_type;
}

QUuid DataFrame::id() const {
    return m_id;
}

QByteArray DataFrame::data() const {
    return m_data;
}

void DataFrame::setType(Type type) {
    m_type = type;
}

void DataFrame::setData(const QByteArray &data) {
    m_data = data;
}

#include <QJsonObject>
#include <QJsonDocument>

QString DataFrame::toString() const {
    QJsonObject obj;
    obj["type"] = type();
    obj["id"] = id().toString();
    obj["data"] = QString::fromUtf8(data());
    QJsonDocument doc(obj);
    return doc.toJson(QJsonDocument::Compact);
}