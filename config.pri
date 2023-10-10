PROJECT_ROOT_PATH = $${PWD}/

CONFIG += silent
CONFIG -= debug_and_release debug_and_release_target

CONFIG(debug, debug|release) {
    BUILD_FLAG = debug
    LIB_SUFFIX = d
    QMAKE_CFLAGS   += -O0
    QMAKE_CXXFLAGS += -O0
} else {
    BUILD_FLAG = release
    QMAKE_CFLAGS += -DNDEBUG
    QMAKE_CXXFLAGS += -DNDEBUG
}

# Берем значение внешней переменной окружения BERKELEY_PATH
BERKELEY_PATH=$$(BERKELEY_PATH)

!isEmpty(BERKELEY_PATH) {
    PATH_TO_BERKELEY_LIB = $${BERKELEY_PATH}/lib
    PATH_TO_BERKELEY_INC = $${BERKELEY_PATH}/include
}

LIBS_PATH = $${PROJECT_ROOT_PATH}/lib
INC_PATH = $${PROJECT_ROOT_PATH}/include           \
           $${PATH_TO_BERKELEY_INC}

BIN_PATH = $${PROJECT_ROOT_PATH}/bin/$${BUILD_FLAG}
STYLES_PATH = $${BIN_PATH}/styles
I18N_PATH = $${BIN_PATH}/i18n
PLUGINS_PATH = $${BIN_PATH}/plugins
DLL_PATH = $${BIN_PATH}/lib
TOOLS_PATH = $${BIN_PATH}/tools
TESTS_PATH = $${BIN_PATH}/tests

BUILD_PATH = $${PROJECT_ROOT_PATH}/build/$${BUILD_FLAG}/$${TARGET}
RCC_DIR = $${BUILD_PATH}/rcc
UI_DIR = $${BUILD_PATH}/ui
MOC_DIR = $${BUILD_PATH}/moc
OBJECTS_DIR = $${BUILD_PATH}/obj

LIBS += -L$${LIBS_PATH}
!isEmpty(PATH_TO_BERKELEY_LIB): LIBS += -L$${PATH_TO_BERKELEY_LIB}


unix {
    DEFINES += HAVE_CLOCK_SETTIME
}

#
# Инициализация генератора случайных чисел для повторяемости
#
QMAKE_CFLAGS += -frandom-seed=flatfish
QMAKE_CXXFLAGS += -frandom-seed=flatfish

#
# Отключаем избыточные warning'и
#
QMAKE_CFLAGS_WARN_ON = -Wall
QMAKE_CXXFLAGS_WARN_ON = -Wall
