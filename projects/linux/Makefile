# Create all necessary folders
ifeq ($(debug), true)
	CREATE_FOLDERS:=$(shell mkdir ..\..\build\debug 2> nul && mkdir ..\..\build\debug\obj 2> nul)
else
	CREATE_FOLDERS:=$(shell mkdir ..\..\build\release 2> nul && mkdir ..\..\build\release\obj 2> nul)
endif

#
CC=gcc
CXX=g++

INCLUDES=-I../../src
OPTIMIZE=-O3
CFLAGS=$(OPTIMIZE) $(INCLUDES) -Wall -Wextra -MMD
CXXFLAGS=-std=c++14 $(OPTIMIZE) $(INCLUDES) -Wall -Wextra -Wno-psabi -MMD
LDLIBS=
LDFLAGS=

APP_BIN_NAME=smacro.exe

#-----------------------------------------------------------------
ifeq ($(debug), true)
	OPTIMIZE=-O0 -g3
	LDFLAGS+=-g3
	APP_BIN=../../build/debug/$(APP_BIN_NAME)
	OBJ_DIR=../../build/debug/obj
else
	APP_BIN=../../build/release/$(APP_BIN_NAME)
	OBJ_DIR=../../build/release/obj
endif

#-----------------------------------------------------------------
# Default target
#-----------------------------------------------------------------
all: app-binary

.PHONY: all clean

#-----------------------------------------------------------------
# Sources
#-----------------------------------------------------------------
APP_SRCS=../../src/main.cpp \
	../../src/processor.cpp \
	../../src/utils.cpp
APP_OBJS=$(subst ../../src/,$(OBJ_DIR)/,$(subst .cpp,.o,$(APP_SRCS)))

$(OBJ_DIR)/%.o: ../../src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(APP_BIN): $(APP_OBJS)
	$(CXX) $(LDFLAGS) -o $(APP_BIN) $(LDLIBS) $(APP_OBJS)
ifneq ($(debug), true)
	strip $(APP_BIN)
endif

app-binary: $(APP_BIN)

#-----------------------------------------------------------------
# Dependencies (recompile when headers have changed)
#-----------------------------------------------------------------
DEPENDENCIES=$(APP_OBJS:%.o=%.d)

-include $(DEPENDENCIES)

clean:
	del /S /Q ..\..\build\*.* > nul 2> nul
	for /D %%i in (..\..\build\*.*) do rmdir /S /Q %%i > nul 2> nul
