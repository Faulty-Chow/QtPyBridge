//
// Created by 87431 on 2025/3/25.
//

#include "PythonIO.h"
#include <QEventLoop>
#include <QTimer>

using namespace QPB;

Session::Ptr PythonIO::createSession() {
    if (not isOpen())
        return nullptr;
    auto session = std::make_shared<Session>();
    QObject::connect(session.get(), &Session::dataSent, this, &PythonIO::send);
    QObject::connect(this, &PythonIO::frameReceived, session.get(), &Session::onFrameReceive);
    return session;
}

bool PythonIO::send(DataFrame::ConstPtr frame) {
    auto packet = frame->toPacket();
    m_wBuffer.append(packet);
    return write(packet);
}

int PythonIO::read(const QByteArray& data) {
    m_rBuffer.append(data);
}

DataFrame::ConstPtr PythonIO::waitForFrame(int msecs) {
    DataFrame::ConstPtr frame;
    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);
    timer.setInterval(msecs);
    QObject::connect(this, &PythonIO::frameReceived, &loop,
                     [&](auto f) {
                         frame = f;
                         loop.quit();
                     });
    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start();
    loop.exec();
    return frame;
}