

#
# This make variable must be set before the demos or examples
# can be built. 
#

#TARGET =arm

TARGET =linux_pc


TARGET_LIB = mspu 

PUSDK_BIN = putest
DIR_PWD=$(PWD)
PWD_DIR = $(DIR_PWD)
PUSDK_DIR = $(PWD_DIR)/bin
INCLUDES_DIR = $(PWD_DIR)/inc

OS_INCLUDES = $(INCLUDES_DIR)/os 
MSPU_INCLUDES = $(INCLUDES_DIR)/mspu
GBPU_INCLUDES = $(INCLUDES_DIR)/gbpu
OSIP_INCLUDES = $(INCLUDES_DIR)/sip
GLOBEL_INCLUDES = $(INCLUDES_DIR)/globel

OUTPUT_PATH   = $(PWD_DIR)/src/objs


CC = g++ -g
GCC = $(CC)
AR = ar -rv


ifeq ($(TARGET), linux_pc) 
GCC = $(CC) 
LIB_DIR = $(PWD_DIR)/lib/
endif

ifeq ($(TARGET), arm)
#CROSS_COMPILE = arm-hismall-linux-
CROSS_COMPILE =arm_v5t_le-
GCC = $(CROSS_COMPILE)$(CC)
LIB_DIR = $(PWD_DIR)/armlib/
endif


LIB_FORMAT =static
#LIB_FORMAT =dynamic

ifeq ($(LIB_FORMAT), dynamic)
LIB_INSTALL = /usr/lib/
MSPU_TARGET = libmspu.so
CFLAGS = -D__MS_DEBUG__  -Wall
LINKLIB_FLAGS = -fPIC -shared
LIB_GCC = $(GCC) $(LINKLIB_FLAGS)
endif

ifeq ($(LIB_FORMAT), static)
LIB_INSTALL = $(LIB_DIR)
MSPU_TARGET = libmspu.a
CFLAGS = -g -Wall
LINKLIB_FLAGS = $(CFLAGS)
LIB_GCC = $(CROSS_COMPILE)$(AR) 
endif

