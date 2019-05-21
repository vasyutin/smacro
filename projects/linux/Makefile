# Create all necessary folders
CREATE_FOLDERS:=$(shell mkdir -p ../../build/obj)

#
CC=gcc
CXX=g++

INCLUDES=-I../../src
OPTIMIZE=-O3
CFLAGS=$(OPTIMIZE) $(INCLUDES) -Wall -Wextra -MMD
CXXFLAGS=-std=c++14 $(OPTIMIZE) $(INCLUDES) -Wall -Wextra -Wno-psabi -MMD
LDLIBS=
LDFLAGS=

APP_BIN_NAME=smacro
APP_BIN=../../build/$(APP_BIN_NAME)
OBJ_DIR=../../build/obj

#-----------------------------------------------------------------
ifeq ($(debug), true)
	OPTIMIZE=-O0 -g3
	LDFLAGS+=-g3
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

app-binary: $(APP_BIN)

#-----------------------------------------------------------------
# Dependencies (recompile when headers have changed)
#-----------------------------------------------------------------
DEPENDENCIES=$(APP_OBJS:%.o=%.d)

-include $(DEPENDENCIES)

clean:
	rm -rf ../../build/*

install: $(APP_BIN)
	install --mode 755 -s -t /usr/bin $(APP_BIN)

uninstall:
	rm -f /usr/bin/$(APP_BIN_NAME)
