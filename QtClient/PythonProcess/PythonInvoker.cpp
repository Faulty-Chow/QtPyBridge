//
// Created by 87431 on 2025/3/25.
//

#include "PythonInvoker.h"
#include <QDebug>
#include "../IPC/PythonIO.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QTimer>

using namespace QPB;

void PythonInvoker::start() {
}

void PythonInvoker::execute(PythonIO *io) {
    if (nullptr == io) {
        qCritical() << "PythonIO is null.";
        return;
    }
    if (not io->isOpen()) {
        qCritical() << "PythonIO is not open.";
        return;
    }
    emit stateChanged(Running);

    QJsonObject request;
    request[ID] = m_id.toString();
    toJson(request);
    QJsonDocument doc(request);
    QByteArray data = doc.toJson();
    auto frame = std::make_shared<DataFrame>(DataFrame::Request, data);

    QObject::connect(io, &PythonIO::errorOccurred, this,
                     [this](int error) {
                         emit errorOccurred(error, "PythonIO error occurred.");
                     });
    QObject::connect(io, &PythonIO::frameReady, this,
                     [this](auto frame) {
                         auto data = frame->data();
                         QJsonParseError error;
                         QJsonDocument doc = QJsonDocument::fromJson(data, &error);
                         if (error.error != QJsonParseError::NoError) {
                             return;
                         }
                         auto response = doc.object();
                         if (not response.contains(ID)) {
                             qWarning() << "Missing ID in response.";
                             return;
                         }
                         if (m_id != QUuid(response[ID].toString())) {
                             return;
                         }

                         switch (frame->type()) {
                             case DataFrame::Response:
                                 emit stateChanged(Completed);
                                 break;
                             case DataFrame::Exception: {
                                 int errCode = response[ErrCode].toInt();
                                 QString errMsg = response[ErrMsg].toString();
                                 emit errorOccurred(errCode, errMsg);
                                 emit stateChanged(Failed);
                                 break;
                             }
                         }
                     });

    QTimer* timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(m_timeout);
    QObject::connect(timer, &QTimer::timeout, this,
                     [this]() {
                         emit errorOccurred(Timeout, "Timeout.");
                         emit stateChanged(Failed);
                     });

    io->write(*frame);
    timer->start();
}
