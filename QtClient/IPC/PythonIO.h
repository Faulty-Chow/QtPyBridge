//
// Created by 87431 on 2025/3/25.
//

#ifndef QTPYBRIDGE_PYTHONIO_H
#define QTPYBRIDGE_PYTHONIO_H

#include <QObject>
#include "Session.h"
#include <QBuffer>

namespace QPB {
    class PythonIO : public QObject {
    Q_OBJECT

    public:
        enum ErrorCode {
            NoError,
        };

        virtual void open() = 0;

        virtual bool waitForConnected(int msecs) = 0;

        virtual bool isOpen() = 0;

        virtual void close() = 0;

        Session::Ptr createSession();

        bool send(DataFrame::ConstPtr frame);

        DataFrame::ConstPtr waitForFrame(int msecs);

        virtual QString errorString() const = 0;

    signals:
        void connected();

        void disconnected();

        void errorOccurred(int errCode, QString errMsg = QString());

        void frameReceived(DataFrame::ConstPtr frame);

    protected:
        virtual int write(const QByteArray& data) = 0;

        int read(const QByteArray& data);

    private:
        QByteArray m_wBuffer, m_rBuffer;
    };
} // QPB

#endif //QTPYBRIDGE_PYTHONIO_H
