cmake_minimum_required(VERSION 3.16)

project(2048 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 找到Qt组件，只需要一次查找即可
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

# 添加include目录到包含路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# 统一定义所有源文件
set(PROJECT_SOURCES
    main.cpp
    include/gameboard.h src/gameboard.cpp
    include/cell.h src/cell.cpp
    include/game.h src/game.cpp
    include/mainwindow.h src/mainwindow.cpp
    include/login.h src/login.cpp
    include/database.h src/database.cpp
)

# 定义可执行文件目标
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(2048
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
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# 链接Qt库（只需要一次链接语句）
target_link_libraries(2048 PRIVATE 
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

# 设置应用程序属性
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.2048)
endif()

set_target_properties(2048 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# 安装目标
include(GNUInstallDirs)
install(TARGETS 2048
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 对于Qt6，完成可执行文件的设置
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(2048)
endif()

# 确保SQL驱动能够正确部署
# 对于Windows平台，复制Qt SQL驱动到插件目录
if(WIN32)
    # 获取Qt库路径
    get_target_property(_qmake_executable Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    
    # 从bin目录转到plugins目录
    get_filename_component(_qt_root_dir "${_qt_bin_dir}" DIRECTORY)
    set(_qt_plugins_dir "${_qt_root_dir}/plugins")
    
    # 添加自定义命令，复制SQL驱动到目标目录
    add_custom_command(TARGET 2048 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory 
            "$<TARGET_FILE_DIR:2048>/plugins/sqldrivers"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${_qt_plugins_dir}/sqldrivers/qsqlite$<$<CONFIG:Debug>:d>.dll"
            "$<TARGET_FILE_DIR:2048>/plugins/sqldrivers/"
        COMMENT "Copying SQLite plugin to output directory"
    )
endif()