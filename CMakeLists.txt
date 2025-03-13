cmake_minimum_required(VERSION 3.5)

project(MotorAssistant VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt6 组件
find_package(Qt6 COMPONENTS Widgets SerialPort PrintSupport REQUIRED)

# 添加 QCustomPlot 库
add_subdirectory(qcustomplot)

# 按模块组织源文件
set(CORE_SOURCES
    HardwareManager/SerialManager.cpp
    HardwareManager/SerialManager.h
    Core/ProtocolHandler.cpp
    Core/ProtocolHandler.h
)

set(THREADS_SOURCES
    Threads/IOThread.cpp
    Threads/IOThread.h
    Threads/DataProcessorThread.cpp
    Threads/DataProcessorThread.h
)

set(WIDGETS_SOURCES
    Widgets/WaveformWidget.cpp
    Widgets/WaveformWidget.h
)

set(MAINWINDOW_SOURCES
    MainWindow/mainwindow.cpp
    MainWindow/mainwindow.h
    MainWindow/mainwindow.ui
)

# 合并所有源文件
set(PROJECT_SOURCES
    main.cpp
    ${CORE_SOURCES}
    ${THREADS_SOURCES}
    ${WIDGETS_SOURCES}
    ${MAINWINDOW_SOURCES}
)

# 包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Core
    ${CMAKE_CURRENT_SOURCE_DIR}/HardwareManager
    ${CMAKE_CURRENT_SOURCE_DIR}/Threads
    ${CMAKE_CURRENT_SOURCE_DIR}/Widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/MainWindow
    ${CMAKE_CURRENT_SOURCE_DIR}/qcustomplot
)

# 添加可执行文件
qt_add_executable(MotorAssistant
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

# 链接库
target_link_libraries(MotorAssistant PRIVATE
    Qt6::Widgets
    Qt6::PrintSupport
    Qt6::SerialPort
    qcustomplot
)

# 设置目标属性
set_target_properties(MotorAssistant PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# 安装配置
include(GNUInstallDirs)
install(TARGETS MotorAssistant
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 最终化可执行文件
qt_finalize_executable(MotorAssistant)
