//
// Created by 87431 on 2025/3/25.
//

#ifndef QTPYBRIDGE_PYTHONINVOKER_H
#define QTPYBRIDGE_PYTHONINVOKER_H

#include <QObject>
#include "../IPC/Session.h"

namespace QPB {
    class PythonIO;

    class PythonInvoker : public QObject {
        Q_OBJECT

    public:
        static inline const QString ID = "id";
        static inline const QString ErrCode = "errCode";
        static inline const QString ErrMsg = "errMsg";

        enum State {
            Idle,
            Running,
            Completed,
            Failed,
            Cancelled
        };

        enum ErrorCode {
            NoError,
            Timeout,
            ParseError,
        };

        explicit PythonInvoker(Session::Ptr session);

        State state() const;

        void start();

        void cancel();

        virtual void invoke() = 0;

    signals:
        void stateChanged(State state);

        void errorOccurred(int errCode, const QString &errMsg);

    protected:
        int m_timeout = 3000;
        Session::Ptr m_session;

    protected slots:
        virtual void onDataReceived(DataFrame::ConstPtr frame);
    };
} // QPB

#endif //QTPYBRIDGE_PYTHONINVOKER_H
