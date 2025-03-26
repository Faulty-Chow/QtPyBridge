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
    if (m_session == nullptr) {
        qCritical() << "Session is null.";
        return;
    }
    invoke();
}

void PythonInvoker::onDataReceived(DataFrame::ConstPtr frame) {
    if (frame->id() != m_session->id()) {
        return;
    }
    switch (frame->type()) {
        case DataFrame::Notification: {
            auto data = frame->data();
            if(data.contains("processing"))
                emit stateChanged(Running);
            break;
        }
        case DataFrame::Exception: {
            auto data = frame->data();
            QJsonParseError error;
            QJsonDocument doc = QJsonDocument::fromJson(data, &error);
            if (error.error != QJsonParseError::NoError) {
                emit errorOccurred(ParseError, "Failed to parse exception.");
                return;
            }
            auto response = doc.object();
            if(not response.contains(ErrCode) or not response.contains(ErrMsg)) {
                emit errorOccurred(ParseError, "Missing ErrCode or ErrMsg.");
                return;
            }
            int errCode = response[ErrCode].toInt();
            QString errMsg = response[ErrMsg].toString();
            emit errorOccurred(errCode, errMsg);
            break;
        }
    }
}
