//
// Created by 87431 on 2025/3/25.
//

#ifndef QTPYBRIDGE_PYTHONIO_H
#define QTPYBRIDGE_PYTHONIO_H

#include <QObject>
#include "../IPC/DataFrame.h"

namespace QPB {
    class PythonIO : public QObject {
        Q_OBJECT

    public:
        enum State {
        };

        enum Attribute {
            Host,
            Port,
            Timeout,
        };

        virtual bool set(Attribute attribute, const QVariant &data) = 0;

        virtual void open() = 0;

        virtual bool waitForConnected(int msecs) = 0;

        virtual bool isOpen() = 0;

        virtual void close() = 0;

        virtual bool write(const DataFrame &frame) = 0;

    signals:
        void frameReady(DataFrame::ConstPtr frame);

        void connected();

        void disconnected();

        void errorOccurred(int error);
    };
} // QPB

#endif //QTPYBRIDGE_PYTHONIO_H
