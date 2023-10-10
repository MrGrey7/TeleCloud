include( config.pri )
include( mkdir.pri )
include( features/features.pri )

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

