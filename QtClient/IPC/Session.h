//
// Created by 87431 on 2025/3/26.
//

#ifndef QTPYBRIDGE_SESSION_H
#define QTPYBRIDGE_SESSION_H

#include <QObject>
#include "DataFrame.h"

namespace QPB {

    class Session : public QObject {
    Q_OBJECT

    public:
        using Ptr = std::shared_ptr<Session>;

        const QUuid &id() const;

        virtual void open();

        virtual void close();

        virtual bool isOpen();

        void send(DataFrame::ConstPtr frame);

        void request(const QByteArray &data);

        void response(const QByteArray &data);

        void notification(const QByteArray &data);

        void exception(const QByteArray &data);

    signals:
        void dataSent(DataFrame::ConstPtr frame);

        void dataReceived(DataFrame::ConstPtr frame);

    public slots:
        void onFrameReceive(DataFrame::ConstPtr frame);

    private:
        QUuid m_id;
    };

} // QPB

#endif //QTPYBRIDGE_SESSION_H
