//
// Created by 87431 on 2025/3/25.
//

#ifndef QTPYBRIDGE_DATAFRAME_H
#define QTPYBRIDGE_DATAFRAME_H

#include <QBuffer>
#include <QByteArray>
#include <QUuid>

namespace QPB {
    class DataFrame {
    public:
        using Ptr = std::shared_ptr<DataFrame>;
        using ConstPtr = std::shared_ptr<const DataFrame>;

        static inline const quint16 MAGIC = 0xCC77;
        static inline const quint8 VERSION = 0x10;

        enum Type {
            Control = 0,
            Heartbeat,
            Request,
            Response,
            Notification,
            Exception,
        };

        DataFrame(QUuid id = QUuid::createUuid());

        static int fromPacket(QBuffer &buffer, DataFrame &frame);

        QByteArray toPacket() const;

        quint8 type() const;

        QUuid id() const;

        QByteArray data() const;

        void setType(Type type);

        void setData(const QByteArray &data);

        QString toString() const;

    private:
        QUuid m_id;
        int m_type{-1};
        QByteArray m_data;
    };
} // QPB

#endif //QTPYBRIDGE_DATAFRAME_H
