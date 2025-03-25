//
// Created by 87431 on 2025/3/25.
//

#ifndef QTPYBRIDGE_PYTHONPROCESSPRIVATE_H
#define QTPYBRIDGE_PYTHONPROCESSPRIVATE_H

#include "PythonProcess.h"

class QProcess;

namespace QPB {

    class PythonThread;

    class PythonProcessPrivate {
    public:

        void setState(PythonProcess::State state);

        bool start();

        void stop();

        PythonInvoker* invoke();

    private:
        PythonProcess *m_q;
        PythonProcess::State m_state;

        QString m_pyPath;
        QStringList m_args;
        QProcess *m_process;
        PythonThread *m_thread;
    };

} // QPB

#endif //QTPYBRIDGE_PYTHONPROCESSPRIVATE_H
