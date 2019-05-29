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
	BIN_DIR_W=..\..\build\debug
	OBJ_DIR_W=..\..\build\debug\obj
else
	BIN_DIR=../../build/release
	APP_BIN=../../build/release/$(APP_BIN_NAME)
	OBJ_DIR=../../build/release/obj
	BIN_DIR_W=..\..\build\release
	OBJ_DIR_W=..\..\build\release\obj
endif

MKDIR_OBJ=@if not exist $(OBJ_DIR_W) mkdir $(OBJ_DIR_W) 2> nul
MKDIR_BIN=@if not exist $(BIN_DIR_W) mkdir $(BIN_DIR_W) 2> nul

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
	$(MKDIR_OBJ)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(APP_BIN): $(APP_OBJS)
	$(MKDIR_BIN)
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
	rmdir /S /Q ..\..\build
