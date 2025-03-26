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

        void open() override;

        bool waitForConnected(int msecs) override;

        bool isOpen() override;

        void close() override;

        bool write(DataFrame::ConstPtr frame) override;

        QString errorString() const override;

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
