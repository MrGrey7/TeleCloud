
include( ../common.pri )

win32 {
    CFG_DEST_PATH = $$replace( BIN_PATH, /, \\ )
    QSS_SRC_PATH  = $$replace( PWD, /, \\ )\\styles
    QSS_DEST_PATH = $$replace( STYLES_PATH, /, \\ )
}

unix {
    CFG_DEST_PATH = $${BIN_PATH}
    QSS_SRC_PATH  = $${PWD}/styles
    QSS_DEST_PATH = $${STYLES_PATH}
}

#
# Копирование конфигурационных файлов в bin
#

TEMPLATE = subdirs
SUBDIRS =

system( $${QMAKE_COPY} *.types $${CFG_DEST_PATH} )
system( $${QMAKE_COPY} *.cfg $${CFG_DEST_PATH} )
system( $${QMAKE_COPY} *.ini $${CFG_DEST_PATH} )
system( $${QMAKE_COPY} *.xml $${CFG_DEST_PATH} )

win32 {
    system( $${QMAKE_COPY} *.bat $${BIN_PATH} )
    system( $${QMAKE_COPY} styles/default-win.qss $${QSS_DEST_PATH}/default.qss )
}

unix {
    system( $${QMAKE_COPY} *.sh $${BIN_PATH} )
    system( $${QMAKE_COPY} styles/default-unix.qss $${QSS_DEST_PATH}/default.qss )
}


SOURCES += \
    *.types \
    *.cfg \
    *.xml \
    *.ini \
    styles/*.qss \
    *.sh
