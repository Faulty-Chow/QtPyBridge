//
// Created by 87431 on 2025/3/25.
//

#ifndef QTPYBRIDGE_DATAFRAME_H
#define QTPYBRIDGE_DATAFRAME_H

#include <QtGlobal>
#include <QByteArray>

namespace QPB {

    class DataFrame {
    public:
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

        struct Header {
            quint16 magic = MAGIC;
            quint8 version = VERSION;
            quint8 type;
            quint32 length;
            quint16 reserve = 0;
        };

        DataFrame(quint8 type, const QByteArray &data = QByteArray());

        DataFrame(const QByteArray &rawData, quint16 magic = 0x55AA, quint8 version = 0x10);

        QByteArray toBytes() const;

        quint8 type() const;

        QByteArray data() const;

        void setData(const QByteArray &data);

        bool isValid() const;

    private:
        Header m_header;
        QByteArray m_data;
    };

} // QPB

#endif //QTPYBRIDGE_DATAFRAME_H
