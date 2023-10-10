
DESTDIR = $${BIN_PATH}/

unix {
	LD_LIBRARY_PATH=$$(LD_LIBRARY_PATH)
	isEmpty(LD_LIBRARY_PATH) {
		LD_LIBRARY_PATH="./lib:."
	}
	# Загрузка *.so из каталогов, указанных в переменной окружения 'LD_LIBRARY_PATH'
	QMAKE_LFLAGS += -Wl,-rpath=$${LD_LIBRARY_PATH}
}
