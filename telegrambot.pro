include( config.pri )
include( mkdir.pri )
#include( features/features.pri )

TEMPLATE = subdirs

#SUBDIRS += \
#    cache.pro

MODULES = config src tools
for( mod, MODULES ) {
    SUBDIRS += $${mod}
#    $${mod}.depends = cache.pro
}

tests {
    SUBDIRS += tests
    tests.depends = src
}

HEADERS += $$files( include/*.h )
HEADERS += $$files( include/Q* )

OTHER_FILES += $$files( *.pri )

QMAKE_CXXFLAGS += -Wno-inconsistent-missing-override \
                  -Wno-unknown-pragmas

greaterThan(QT_MAJOR_VERSION, 6): DEFINES += QT_DISABLE_DEPRECATED_UP_TO=0x050F00

