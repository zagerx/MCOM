cmake_minimum_required(VERSION 3.5)

project(MotorAssistant VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Qt组件（必须包含SerialPort）
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets SerialPort)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets SerialPort)

# 添加QCustomPlot库（需确保qcustomplot目录存在）
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

# set(WIDGETS_SOURCES
#     Widgets/WaveformWidget.cpp
#     Widgets/WaveformWidget.h
# )

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
    # ${CMAKE_CURRENT_SOURCE_DIR}/qcustomplot
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MotorAssistant
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(MotorAssistant SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(MotorAssistant
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# 链接库（必须包含SerialPort和QCustomPlot）
target_link_libraries(MotorAssistant PRIVATE 
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::SerialPort
#    qcustomplot
)

# 设置目标属性
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MotorAssistant)
endif()

set_target_properties(MotorAssistant PROPERTIES
    ${BUNDLE_ID_OPTION}
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

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MotorAssistant)
endif()
