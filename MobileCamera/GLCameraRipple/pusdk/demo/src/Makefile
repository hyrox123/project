# Makefile
#
ROOTDIR = ../..
include $(ROOTDIR)/Rules.make 

CFLAGS += -c -D__MS_DEBUG__
SYSLIB = -lpthread  -lrt
LIB_LOCAL_DYNAMIC = -lmspu -leXosip2 -losip2 -losipparser2 -lresolv
LIB_LOCAL_STATIC = $(LIB_DIR)libmspu.a      $(LIB_DIR)libeXosip2.a $(LIB_DIR)libosip2.a $(LIB_DIR)libosipparser2.a $(LIB_DIR)libresolv.a
LIB_INCLUDES = -I$(GLOBEL_INCLUDES) 

DEMO_INCLUDES = -I../inc/
CFLAGS += $(DEMO_INCLUDES) 

SRC  = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRC))

all:$(PUSDK_BIN)

ifeq ($(LIB_FORMAT), dynamic)
$(PUSDK_BIN):$(OBJS)
	$(GCC) $(LIB_INCLUDES) $(SYSLIB) -L$(LIB_DIR) $(LIB_LOCAL_DYNAMIC)  $^ -o $@ -lpthread
	cp $(PUSDK_BIN) $(PWD_DIR) -rf
%.o:%.c
	$(GCC) $(CFLAGS) $< -o $@ -lpthread $(LIB_INCLUDES)
endif

ifeq ($(LIB_FORMAT), static)
$(PUSDK_BIN):$(OBJS)
	$(GCC) -o $@ $(OBJS) $(LIB_LOCAL_STATIC) $(SYSLIB) 
	cp $(PUSDK_BIN) $(PWD_DIR) -rf
%.o:%.c
	$(GCC) $< -o $@ $(CFLAGS) $(LIB_INCLUDES)
endif

install:$(PUSDK_BIN)
	@echo $(BIN_DIR)
	cp $(PUSDK_BIN) $(PUSDK_DIR) -rf

clean:
	rm -rf $(OBJS) $(PUSDK_BIN) 
