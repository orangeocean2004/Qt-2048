#### filepath: d:\Code\Qt\2048\CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

project(2048 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 找到Qt组件
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# 添加include目录到包含路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# 定义资源文件
set(APP_ICON_RESOURCE "")
if(WIN32)
    # 这里指定你要使用的 .rc 文件
    set(APP_ICON_RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/logo.rc")
endif()

# 统一定义所有源文件
set(PROJECT_SOURCES
    main.cpp
    include/gameboard.h src/gameboard.cpp
    include/cell.h src/cell.cpp
    include/game.h src/game.cpp
    include/mainwindow.h src/mainwindow.cpp
    include/login.h src/login.cpp
    include/database.h src/database.cpp
    # 如果在 Qt6 上使用 qt_add_executable，需要把资源文件也加到这里
    ${APP_ICON_RESOURCE}
)

# 定义可执行文件目标
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    # 注意加上 ${APP_ICON_RESOURCE}
    qt_add_executable(2048
        WIN32                  # WIN32 选项可隐藏控制台窗口
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(2048 SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(2048
            WIN32
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# 链接Qt库
target_link_libraries(2048 PRIVATE 
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

# 设置应用程序属性
set_target_properties(2048 PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# 安装目标
install(TARGETS 2048
    RUNTIME DESTINATION .
    LIBRARY DESTINATION .
)

# 如果是 Qt6，完成可执行文件的设置
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(2048)
endif()

# 复制一下 SQLite 驱动（如有必要）
if(WIN32)
    get_target_property(_qmake_executable Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    get_filename_component(_qt_root_dir "${_qt_bin_dir}" DIRECTORY)
    set(_qt_plugins_dir "${_qt_root_dir}/plugins")

    add_custom_command(TARGET 2048 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory 
            "$<TARGET_FILE_DIR:2048>/plugins/sqldrivers"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${_qt_plugins_dir}/sqldrivers/qsqlite$<$<CONFIG:Debug>:d>.dll"
            "$<TARGET_FILE_DIR:2048>/plugins/sqldrivers/"
        COMMENT "Copying SQLite plugin to output directory"
    )
endif()