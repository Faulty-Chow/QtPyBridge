//
// Created by 87431 on 2025/3/25.
//

#include "PythonThread.h"
#include "../IPC/PythonIO.h"
#include "PythonProcessPrivate.h"
#include <QTimer>
#include <QDebug>

#include "PythonInvoker.h"

namespace QPB {
    void PythonThread::run() {
        QTimer retryTimer;
        PythonIO *pio;

        QObject::connect(pio, &PythonIO::connected,
                         [&]() {
                             qInfo() << "Socket connected.";
                             retryTimer.stop();
                         });
        QObject::connect(pio, &PythonIO::connected, this,
                         [this]() {
                             m_p->setState(PythonProcess::Running);
                         });
        QObject::connect(pio, &PythonIO::disconnected,
                         [&]() {
                             qInfo() << "Socket disconnected.";
                             retryTimer.start();
                         });
        QObject::connect(pio, &PythonIO::disconnected, this,
                         [this]() {
                             m_p->setState(PythonProcess::Error);
                         });
        QObject::connect(pio, &PythonIO::errorOccurred, this,
                         [=]() {
                             m_p->setState(PythonProcess::Error);
                         });
        QObject::connect(pio, &PythonIO::errorOccurred,
                         [=](auto error) {
                             //                             qCritical() << "Socket error: " << pio->errorString();
                         });


        QObject::connect(&retryTimer, &QTimer::timeout,
                         [&]() {
                             if (isInterruptionRequested()) {
                                 retryTimer.stop();
                                 return;
                             }
                             if (not pio->isOpen()) {
                                 qInfo() << "Attempting to connect.";
                                 pio->open();
                             }
                         });

        QObject::connect(this, &PythonThread::invoked, pio,
                         [&](PythonInvoker *invoker) {
                             if (this != invoker->thread()) {
                                 qWarning() << "Invoker is not running in this thread.";
                             }

                         });

        retryTimer.setInterval(5000);
        retryTimer.start();


        exec();
    }
} // QPB
