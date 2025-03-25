//
// Created by 87431 on 2025/3/25.
//

#ifndef QTPYBRIDGE_PYTHONTHREAD_H
#define QTPYBRIDGE_PYTHONTHREAD_H

#include <QThread>
#include "../IPC/DataFrame.h"
#include <QMap>

class QLocalSocket;

class QTcpSocket;

namespace QPB {

    class PythonProcessPrivate;

    class PythonInvoker;

    class PythonThread : public QThread {
    public:
        PythonThread(PythonProcessPrivate *p);

        ~PythonThread() override;

        void run() override;

    public slots:

        void invoke(PythonInvoker *invoker);

    signals:

        void frameReady(const DataFrame &frame);

    private:
        PythonProcessPrivate *m_p;
    };

} // QPB

#endif //QTPYBRIDGE_PYTHONTHREAD_H
