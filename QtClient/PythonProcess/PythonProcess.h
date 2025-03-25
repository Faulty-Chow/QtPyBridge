//
// Created by 87431 on 2025/3/25.
//

#ifndef QTPYBRIDGE_PYTHONPROCESS_H
#define QTPYBRIDGE_PYTHONPROCESS_H

#include <QObject>

namespace QPB {

    class PythonInvoker;

    class PythonProcess : public QObject {
    Q_OBJECT

    public:
        explicit PythonProcess(QObject *parent = nullptr);

        virtual ~PythonProcess() override;

        enum ErrorCode {
            NoError,
            Crashed,
            Timeout,
            IPCError,
            UserError = 10000
        };

        enum State {
            NotRunning,
            Starting,
            Running,
            Error
        };

        void start();

        void stop();

        PythonInvoker* invoke();

    signals:

        void stateChanged(State state);


    };
} // QPB

#endif //QTPYBRIDGE_PYTHONPROCESS_H
