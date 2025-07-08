
TEMPLATE = subdirs

LIBS += \
    libs \

SUBDIRS += $${LIBS}

MODULES = \
    telegrambot-gui \

for( mod, MODULES ) {
    exists( $${PWD}/$${mod} ) {
        SUBDIRS += $${mod}
        # Установка зависимости от библиотек для возможности параллельной сборки
        $${mod}.depends = $${LIBS}
    }
}
