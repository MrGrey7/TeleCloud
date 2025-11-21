set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(APP_NAME TeleCloud)

add_executable(${APP_NAME}
    main.cpp
    telegrambotgui.cpp telegrambotgui.h
    mainwindow.cpp mainwindow.h mainwindow.ui
    dbmanager.cpp dbmanager.h
    botcontroller.cpp botcontroller.h
    messagebroker.cpp messagebroker.h
    types.h
    resources.qrc
)

# FIX: Only depend on DeployStyles if it actually exists
add_dependencies(${APP_NAME} DeployConfigs)
if(TARGET DeployStyles)
    add_dependencies(${APP_NAME} DeployStyles)
endif()

target_link_libraries(${APP_NAME} PRIVATE
    Qt5::Core 
    Qt5::Gui 
    Qt5::Widgets 
    Qt5::Sql 
    Qt5::Network
    Qt5::Xml 
    telegrambotlib-qt-fork
)

if(UNIX)
    set_target_properties(${APP_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN/lib")
endif()