//
// Created by 87431 on 2025/3/25.
//

#include "DataFrame.h"
#include <QBuffer>
#include <QDebug>

using namespace QPB;

DataFrame::DataFrame(quint8 type, const QByteArray &data) {
    m_header.type = type;
    m_header.length = data.size();
    m_data = data;
}

DataFrame::DataFrame(const QByteArray &rawData, quint16 magic, quint8 version) {
    QBuffer buffer;
    buffer.setData(rawData);
    buffer.open(QIODevice::ReadOnly);

    qint64 magicPos = -1;
    const char *dataPtr = rawData.constData();
    const qint64 dataSize = rawData.size();
    for (qint64 i = 0; i <= dataSize - sizeof(Header); ++i) {
        // dataPtr + i may not be aligned to 2 bytes, so we can't use a direct cast here
        if (memcmp(dataPtr + i, &magic, sizeof(magic)) == 0) {
            magicPos = i;
            break;
        }
    }
    if (magicPos == -1) {
        qWarning() << "Magic number not found!";
        return;
    }

    buffer.seek(magicPos);
    if (buffer.bytesAvailable() < sizeof(Header)) {
        qWarning() << "Incomplete header!";
        return;
    }

    buffer.read(reinterpret_cast<char *>(&m_header), sizeof(Header));
    if (buffer.bytesAvailable() < m_header.length) {
        qWarning() << "Incomplete payload! Expected:" << m_header.length
                   << "Actual:" << buffer.bytesAvailable();
        return;
    }
    m_data = buffer.read(m_header.length);
}

QByteArray DataFrame::toBytes() const {
    if (not isValid()) {
        qWarning() << "Invalid data frame!";
        return {};
    }
    QByteArray byteArray;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);
    stream.writeRawData(reinterpret_cast<const char *>(&m_header), sizeof(Header));
    stream.writeRawData(m_data.constData(), m_data.size());
    return byteArray;
}

quint8 DataFrame::type() const {
    return m_header.type;
}

QByteArray DataFrame::data() const {
    return m_data;
}

void DataFrame::setData(const QByteArray &data) {
    m_data = data;
    m_header.length = data.size();
}

bool DataFrame::isValid() const {
    return m_header.length == m_data.size();
}