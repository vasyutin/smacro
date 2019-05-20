# Create all necessary folders
ifeq ($(debug), true)
	CREATE_FOLDERS:=$(shell mkdir ..\..\build\Debug && mkdir ..\..\build\Debug\obj)
else
	CREATE_FOLDERS:=$(shell mkdir ..\..\build\Release && mkdir ..\..\build\Release\obj)
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

#-----------------------------------------------------------------
# Debug options
#-----------------------------------------------------------------
ifeq ($(debug), true)
    OPTIMIZE=-O0 -g3
    LDFLAGS+=-g3
endif

#-----------------------------------------------------------------
# Files to make
#-----------------------------------------------------------------
SMACRO_BIN=bin/wifi-receiver

#-----------------------------------------------------------------
# Default target
#-----------------------------------------------------------------
all: smacro

.PHONY: all clean

#-----------------------------------------------------------------
# Wi-Fi Receiver sources and rules
#-----------------------------------------------------------------
WR_CORE_SRCS=src/main.cpp \
    src/AppConfig.cpp \
    src/ConfigFile.cpp \
    src/JsonRpc.cpp \
    src/DongleAdaptor.cpp \
    src/DongleManager.cpp \
    src/LoggingSystem.cpp
WR_CORE_OBJS=$(subst src/,bin/obj/wr/,$(subst .cpp,.o,$(WR_CORE_SRCS)))

bin/obj/wr/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

#---
WR_NET_SRCS=src/net/SessionController.cpp \
    src/net/IOService.cpp \
    src/net/net_common.cpp \
    src/net/BulletSender.cpp
WR_NET_OBJS=$(subst src/net/,bin/obj/wr/net/,$(subst .cpp,.o,$(WR_NET_SRCS)))

bin/obj/wr/net/%.o: src/net/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

#---
WR_CPR_SRCS=../copypro-lite/src/libcopyprotection-lite.cpp
WR_CPR_OBJS=$(subst ../copypro-lite/src/,bin/obj/wr/cpr/,$(subst .cpp,.o,$(WR_CPR_SRCS)))

bin/obj/wr/cpr/%.o: ../copypro-lite/src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

#---
WR_CRC_SRCS=src/crc/crc_algorithms.c src/crc/crc_computer.c
WR_CRC_OBJS=$(subst src/crc/,bin/obj/wr/crc/,$(subst .c,.o,$(WR_CRC_SRCS)))

bin/obj/wr/crc/%.o: src/crc/%.c
	$(CC) $(CFLAGS) -c $< -o $@

#---
WR_LWCAP_SRCS=../libwcap/src/array_appender.c \
    ../libwcap/src/dump.c \
    ../libwcap/src/libwcap.c \
    ../libwcap/src/netlink_connection.c \
    ../libwcap/src/nm_compat.c \
    ../libwcap/src/wcap_file.c \
    ../libwcap/src/wcap_log_adaptor.c \
    ../libwcap/src/wcap_log.c \
    ../libwcap/src/wcap_queue.c \
    ../libwcap/src/wcap_thread.c \
    ../libwcap/src/wcap_timer.c \
    ../libwcap/src/wifi_channels.c
WR_LWCAP_OBJS=$(subst ../libwcap/src/,bin/obj/wr/libwcap/,$(subst .c,.o,$(WR_LWCAP_SRCS)))

bin/obj/wr/libwcap/%.o: ../libwcap/src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

#---
WR_OBJS=$(WR_CORE_OBJS) $(WR_NET_OBJS) $(WR_CPR_OBJS) $(WR_CRC_OBJS) $(WR_LWCAP_OBJS)

$(WR_BIN): $(WR_OBJS)
	$(CXX) $(LDFLAGS) -o $(WR_BIN) $(LDLIBS) $(WR_OBJS)

wr: $(WR_BIN)

#-----------------------------------------------------------------
# Wi-Fi Receiver Minitor
#-----------------------------------------------------------------
WRM_CXXFLAGS=$(CXXFLAGS)
WRM_CXXFLAGS+=-I./include/monitor

WRM_CORE_SRCS=src/monitor/ApiClientConnection.cpp \
    src/monitor/ApiClient.cpp \
    src/monitor/IfaceModel.cpp \
    src/monitor//wr-monitor.cpp
WRM_CORE_OBJS=$(subst src/monitor/,bin/obj/wrm/,$(subst .cpp,.o,$(WRM_CORE_SRCS)))

bin/obj/wrm/%.o: src/monitor/%.cpp
	$(CXX) $(WRM_CXXFLAGS) -c $< -o $@

#---
WRM_NET_SRCS=src/net/net_common.cpp
WRM_NET_OBJS=$(subst src/net/,bin/obj/wrm/net/,$(subst .cpp,.o,$(WRM_NET_SRCS)))

bin/obj/wrm/net/%.o: src/net/%.cpp
	$(CXX) $(WRM_CXXFLAGS) -c $< -o $@

#---
WRM_OBJS=$(WRM_CORE_OBJS) $(WRM_NET_OBJS)

$(WRM_BIN): $(WRM_OBJS)
	$(CXX) $(LDFLAGS) -o $(WRM_BIN) $(LDLIBS) $(WRM_OBJS)

wrm: $(WRM_BIN)

#-----------------------------------------------------------------
# Machine id sources
#-----------------------------------------------------------------
MID_CORE_SRCS=../copypro-lite/src/utils/machine-id/main.cpp
MID_CORE_OBJS=bin/obj/mid/main.o

bin/obj/mid/%.o: ../copypro-lite/src/utils/machine-id/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

#---
MID_LIB_SRCS=../copypro-lite/src/libcopyprotection-lite.cpp
MID_LIB_OBJS=bin/obj/mid/lib/libcopyprotection-lite.o

bin/obj/mid/lib/%.o: ../copypro-lite/src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

MID_OBJS=$(MID_CORE_OBJS) $(MID_LIB_OBJS)

$(MID_BIN): $(MID_OBJS)
	$(CXX) $(LDFLAGS) -o $(MID_BIN) $(LDLIBS) $(MID_OBJS)

machine-id: $(MID_BIN)

#-----------------------------------------------------------------
# Make License sources
#-----------------------------------------------------------------
MKL_CORE_SRCS=../copypro-lite/src/utils/make-license/main.cpp
MKL_CORE_OBJS=bin/obj/mkl/main.o

bin/obj/mkl/%.o: ../copypro-lite/src/utils/make-license/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

#---
MKL_LIB_SRCS=../copypro-lite/src/libcopyprotection-lite.cpp
MKL_LIB_OBJS=bin/obj/mkl/lib/libcopyprotection-lite.o

bin/obj/mkl/lib/%.o: ../copypro-lite/src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

MKL_OBJS=$(MKL_CORE_OBJS) $(MKL_LIB_OBJS)

$(MKL_BIN): $(MKL_OBJS)
	$(CXX) $(LDFLAGS) -o $(MKL_BIN) $(LDLIBS) $(MKL_OBJS)

make-license: $(MKL_BIN)

#-----------------------------------------------------------------
# Dependencies (recompile when headers have changed)
#-----------------------------------------------------------------
ALL_OBJS=$(WR_OBJS) $(MID_OBJS) $(MKL_OBJS)
DEPENDENCIES=$(ALL_OBJS:%.o=%.d)

-include $(DEPENDENCIES)

clean:
	del /S /Q ..\..\build\*.* > nul 2> nul
	for /D %%i in (..\..\build\*.*) do rmdir /S /Q %%i > nul 2> nul
