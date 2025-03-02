cmake_minimum_required(VERSION 3.5)

# 定义子项目（可选，增强可读性）
project(qcustomplot LANGUAGES CXX)

# 启用Qt元对象系统自动处理
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# 设置C++标准（与主项目一致）
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加QCustomPlot源文件
add_library(qcustomplot STATIC
    qcustomplot.cpp
    qcustomplot.h
)

# 包含头文件路径
target_include_directories(qcustomplot
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}  # 确保主项目能找到qcustomplot.h
)

# 链接Qt依赖项（继承主项目的Qt版本）
target_link_libraries(qcustomplot
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets  # 必须链接Qt Widgets模块
)
