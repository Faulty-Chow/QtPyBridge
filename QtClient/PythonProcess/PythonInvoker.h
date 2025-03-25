//
// Created by 87431 on 2025/3/25.
//

#ifndef QTPYBRIDGE_PYTHONINVOKER_H
#define QTPYBRIDGE_PYTHONINVOKER_H

#include <QObject>

namespace QPB {

    class PythonThread;

    class PythonInvoker : public QObject {
    Q_OBJECT

    public:
        explicit PythonInvoker(PythonThread *thread);

        enum State {
            Idle,
            Running,
            Completed,
            Failed,
            Cancelled
        };

        State state() const;

        void start();

        void cancel();

    protected:
        virtual int write(const QString &msg) = 0;

        virtual int read(QString &msg) = 0;

    signals:
        void stateChanged(State state);

        void errorOccurred(int errCode, const QString &errMsg);

        void msgReceived(const QString &msg);

    private:
        PythonThread* m_thread;
    };

} // QPB

#endif //QTPYBRIDGE_PYTHONINVOKER_H
