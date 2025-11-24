set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(APP_NAME TeleCloud)

# Define sources
set(APP_SOURCES
    main.cpp
    telegrambotgui.cpp telegrambotgui.h
    mainwindow.cpp mainwindow.h mainwindow.ui
    dbmanager.cpp dbmanager.h
    botcontroller.cpp botcontroller.h
    messagebroker.cpp messagebroker.h
    types.h
    resources.qrc
)

if(WIN32)
    # Adjust path if your rc file is in a subfolder
    list(APPEND APP_SOURCES "resources/appicon.rc")
endif()

# Add WIN32 to suppress console, and include the sources with the RC file
add_executable(${APP_NAME} WIN32 ${APP_SOURCES})

add_dependencies(${APP_NAME} DeployConfigs)
if(TARGET DeployStyles)
    add_dependencies(${APP_NAME} DeployStyles)
endif()

target_include_directories(${APP_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(${APP_NAME} PRIVATE
    Qt5::Core 
    Qt5::Gui 
    Qt5::Widgets 
    Qt5::Sql 
    Qt5::Network
    Qt5::Xml 
    telegrambotlib-qt-fork
)

# 1. Install the Executable
install(TARGETS ${APP_NAME}
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION bin
)

# 2. Windows Deployment (Manual Windeployqt)
if(WIN32)
    # Find windeployqt.exe based on the qmake location
    get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

    if(WINDEPLOYQT_EXECUTABLE)
            # Execute windeployqt on the installed .exe
            install(CODE "
                message(STATUS \"Running windeployqt...\")
                execute_process(
                    COMMAND \"${WINDEPLOYQT_EXECUTABLE}\"
                    --release
                    --network
                    --no-translations
                    --no-compiler-runtime
                    --no-opengl-sw
                    --dir \"\${CMAKE_INSTALL_PREFIX}/bin\"
                    \"\${CMAKE_INSTALL_PREFIX}/bin/${APP_NAME}.exe\"
                )
            ")
    else()
        message(WARNING "windeployqt not found! Deployment will be incomplete.")
    endif()
endif()

if(UNIX)
    set_target_properties(${APP_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN/lib")
endif()
