TEMPLATE = app
TARGET = smacro
CONFIG += console
QT += core

CONFIG(release, debug|release) {
	DESTDIR = $${_PRO_FILE_PWD_}/../../build/release
	OBJECTS_DIR = $${_PRO_FILE_PWD_}/../../build/release/obj
	DEFINES += NDEBUG
}

CONFIG(debug, debug|release) {
	DESTDIR = $${_PRO_FILE_PWD_}/../../build/debug
	OBJECTS_DIR = $${_PRO_FILE_PWD_}/../../build/debug/obj
}

INCLUDEPATH += $${_PRO_FILE_PWD_}/../../src
SOURCES += $${_PRO_FILE_PWD_}/../../src/main.cpp \
	$${_PRO_FILE_PWD_}/../../src/processor.cpp

INSTALLS += package
package.path = /usr/local/bin/
win32:package.files += $${DESTDIR}/smacro.exe
unix:package.files += $${DESTDIR}/smacro
package.CONFIG = no_check_exist

