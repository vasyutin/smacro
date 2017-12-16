TEMPLATE = app
TARGET = smacro
CONFIG += console
QT += core

CONFIG(release, debug|release) {
	DESTDIR = $${_PRO_FILE_PWD_}/../../../../build/release
	OBJECTS_DIR = $${OUT_PWD}/release/obj
	DEFINES += NDEBUG
}

CONFIG(debug, debug|release) {
	DESTDIR = $${_PRO_FILE_PWD_}/../../../../build/debug
	OBJECTS_DIR = $${OUT_PWD}/debug/obj
}

INCLUDEPATH += $${_PRO_FILE_PWD_}/../../src
SOURCES += $${_PRO_FILE_PWD_}/../../src/main.cpp \
	$${_PRO_FILE_PWD_}/../../src/processor.cpp

INSTALLS += package
package.path = /usr/local/bin/
package.files += $${DESTDIR}/smacro.exe
package.CONFIG = no_check_exist

