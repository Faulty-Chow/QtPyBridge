cmake_minimum_required(VERSION 3.28)
project(QtPyBridge)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
endif ()

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_PREFIX_PATH "D:/Libs/Qt/Qt-6.6.2/lib/cmake/Qt6")
set(CMAKE_PREFIX_PATH "D:/Libs/Qt/Qt-6.5.2/lib/cmake/Qt6")
find_package(Qt6 REQUIRED COMPONENTS
        Core
        Widgets
        Concurrent
        Network
)
qt_standard_project_setup()

set(SOURCES
        QtClient/IPC/DataFrame.cpp
        QtClient/IPC/PythonIO.cpp
        QtClient/IPC/TcpSocket.cpp
        QtClient/PythonProcess/PythonInvoker.cpp
        QtClient/PythonProcess/PythonProcess.cpp
        QtClient/PythonProcess/PythonProcessPrivate.cpp
        QtClient/PythonProcess/PythonThread.cpp
)

set(HEADERS
        QtClient/IPC/DataFrame.h
        QtClient/IPC/PythonIO.h
        QtClient/IPC/TcpSocket.h
        QtClient/PythonProcess/PythonInvoker.h
        QtClient/PythonProcess/PythonProcess.h
        QtClient/PythonProcess/PythonProcessPrivate.h
        QtClient/PythonProcess/PythonThread.h
)

add_library(${PROJECT_NAME}
        ${SOURCES}
        ${HEADERS}
)

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME}
        Qt6::Core
        Qt6::Widgets
        Qt6::Concurrent
        Qt6::Network
)
