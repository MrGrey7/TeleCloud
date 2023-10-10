TEMPLATE = lib

DESTDIR = $${LIBS_PATH}

isEmpty(DLLDESTDIR) {
    DLLDESTDIR = $${LIBS_PATH}
}

#VERSION = 1.0.0
QMAKE_TARGET_COPYRIGHT = (c) 

# Отключение версионирования разделяемых библиотек
CONFIG += unversioned_libname unversioned_soname
