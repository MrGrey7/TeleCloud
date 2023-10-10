#
# Создание временных директорий для сборки
#

include( config.pri )

DIRS =              \
    $${BIN_PATH}    \
    $${STYLES_PATH} \
    $${I18N_PATH}   \
    $${BUILD_PATH}  \
    $${RCC_DIR}     \
    $${UI_DIR}      \
    $${MOC_DIR}     \
    $${OBJECTS_DIR} \
    $${LIBS_PATH}   \
    $${PLUGINS_PATH}\
    $${TOOLS_PATH}  \
    $${TESTS_PATH}  \

win32 {
    for(dir, DIRS) {
        dir_list += $$replace( dir, /, \\ )
    }
    mkdir_cmd = $$replace( PROJECT_ROOT_PATH, /, \\ )\\tools\\win32\\mkdir.bat
}

unix {
    for(dir, DIRS) {
        dir_list += $$dir
    }
    mkdir_cmd = mkdir -p
}

system( $${mkdir_cmd} $${dir_list} )
