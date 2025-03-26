//
// Created by 87431 on 2025/3/25.
//

#ifndef QTPYBRIDGE_PYTHONINVOKER_H
#define QTPYBRIDGE_PYTHONINVOKER_H

#include <QObject>
#include "../IPC/DataFrame.h"
#include <QUuid>

namespace QPB {
    class PythonIO;

    class PythonInvoker : public QObject {
        Q_OBJECT

    public:
        static inline const QString ID = "id";
        static inline const QString ErrCode = "errCode";
        static inline const QString ErrMsg = "errMsg";

        explicit PythonInvoker();

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
            IPCError,
            UserError = 10000
        };

        State state() const;

        void start();

        void cancel();

        virtual void execute(PythonIO *io);

    signals:
        void stateChanged(State state);

        void errorOccurred(int errCode, const QString &errMsg);

        void msgReceived(const QString &msg);

    protected:
        virtual void toJson(QJsonObject &json) const = 0;

    private:
        int m_timeout = 3000;
        QUuid m_id;
    };
} // QPB

#endif //QTPYBRIDGE_PYTHONINVOKER_H
