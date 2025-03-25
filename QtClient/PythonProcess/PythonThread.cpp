//
// Created by 87431 on 2025/3/25.
//

#include "PythonThread.h"
#include "../IPC/PythonIO.h"
#include "PythonProcessPrivate.h"
#include <QTimer>
#include <QDebug>

namespace QPB {
    void PythonThread::run() {
        QTimer retryTimer;
        PythonIO* pio;

        QObject::connect(pio, &PythonIO::connected, this,
                         [&]() {
                             qInfo() << "Socket connected.";
                             retryTimer.stop();
                             m_p->setState(PythonProcess::Running);
                         });
        QObject::connect(pio, &PythonIO::disconnected, this,
                         [&]() {
                             qInfo() << "Socket disconnected.";
                             retryTimer.start();
                             m_p->setState(PythonProcess::Error);
                         });
        QObject::connect(pio, &PythonIO::errorOccurred, this,
                         [=]() {

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
                             if(not pio->isOpen()){
                                 qInfo() << "Attempting to connect.";
                                 pio->open();
                             }
                         });

        retryTimer.setInterval(5000);
        retryTimer.start();

        exec();
    }
} // QPB