//
// Created by 87431 on 2025/3/25.
//

#include "PythonProcessPrivate.h"
#include <QProcess>
#include <QThread>
#include "PythonThread.h"
#include "PythonInvoker.h"

using namespace QPB;

bool PythonProcessPrivate::start() {
    if (nullptr == m_thread) {
        m_thread = new PythonThread(this);
    }

    if (nullptr == m_process) {
        m_process = new QProcess(m_q);
        QObject::connect(m_process, &QProcess::started, m_thread,
                         [this]() {
                             m_thread->start();
                         });
    }

    if (m_pyPath.endsWith(".py")) {
        m_process->start("python", QStringList() << m_pyPath << m_args);
    } else {
        m_process->start(m_pyPath, m_args);
    }

    return true;
}

PythonInvoker *PythonProcessPrivate::invoke() {
    PythonInvoker *invoker;

    QObject::connect(invoker, &PythonInvoker::stateChanged, m_thread,
    [invoker](PythonInvoker::State state) {
        if (PythonInvoker::Idle == state) {
            m_thread->invoke(invoker);
        }
    });
}