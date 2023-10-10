ROOT = ../..
include( $$ROOT/common.pri )
include( $$ROOT/lib.pri )

include( telegrambotlib-qt-fork/telegrambotlib-qt.pri )

TARGET = telegrambotlib-qt-fork$${LIB_SUFFIX}
TEMPLATE = lib
DLLDESTDIR = $$DLL_PATH
