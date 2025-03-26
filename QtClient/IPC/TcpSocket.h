//
// Created by 87431 on 2025/3/25.
//

#ifndef QTPYBRIDGE_TCPSOCKET_H
#define QTPYBRIDGE_TCPSOCKET_H

#include "PythonIO.h"

class QTcpSocket;

namespace QPB {
    class TcpSocket : public PythonIO {
        Q_OBJECT

    public:
        TcpSocket(QObject *parent = nullptr);

        bool set(Attribute attribute, const QVariant &data) override;

        void open() override;

        bool waitForConnected(int msecs) override;

        bool isOpen() override;

        void close() override;

        bool write(const DataFrame &frame) override;

    private slots:
        void onReadyRead();

    private:
        QTcpSocket *m_socket;
        QByteArray m_buffer;

        QString m_host;
        quint16 m_port;
    };
} // QPB

#endif //QTPYBRIDGE_TCPSOCKET_H
