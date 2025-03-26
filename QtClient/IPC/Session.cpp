//
// Created by 87431 on 2025/3/26.
//

#include "Session.h"
#include <QDebug>

using namespace QPB;

void Session::send(DataFrame::ConstPtr frame) {
    if (frame->id() != m_id) {
        qWarning() << "ID mismatched!";
    }
    emit dataSent(frame);
}

void Session::request(const QByteArray &data) {
    auto frame = std::make_shared<DataFrame>(m_id);
    frame->setType(DataFrame::Request);
    frame->setData(data);
    send(frame);
}

void Session::response(const QByteArray &data) {
    auto frame = std::make_shared<DataFrame>(m_id);
    frame->setType(DataFrame::Response);
    frame->setData(data);
    send(frame);
}

void Session::notification(const QByteArray &data) {
    auto frame = std::make_shared<DataFrame>(m_id);
    frame->setType(DataFrame::Notification);
    frame->setData(data);
    send(frame);
}

void Session::exception(const QByteArray &data) {
    auto frame = std::make_shared<DataFrame>(m_id);
    frame->setType(DataFrame::Exception);
    frame->setData(data);
    send(frame);
}

void Session::onFrameReady(DataFrame::ConstPtr frame) {
    if (frame->id() == m_id) {
        emit dataReceived(frame);
    }
}

