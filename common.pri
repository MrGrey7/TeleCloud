
include( $$PWD/config.pri )

INCLUDEPATH += $${INC_PATH}/

DEPENDPATH += $${INC_PATH}/

#
# Сброс временных меток в PE-заголовке для exe- и dll-файлов
#
win32 {
    patch_cmd = $$replace( PROJECT_ROOT_PATH, /, \\ )\\tools\\win32\\PETool

    # Команда на сброс меток для DLL-библиотеки (Patch_DLL $$TARGET)
    defineReplace(Patch_DLL) {
        return ( $$patch_cmd $$replace( LIBS_PATH, /, \\ )\\$${1}.dll /Destamp )
    }

    # Команда на сброс меток для EXE (Patch_EXE $$TARGET)
    defineReplace(Patch_EXE) {
        return ( $$patch_cmd $$replace( BIN_PATH, /, \\ )\\$${1}.exe /Destamp )
    }
}
