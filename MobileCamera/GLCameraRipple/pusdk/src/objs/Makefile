# Makefile
#
ROOTDIR = ../..
include $(ROOTDIR)/Rules.make 

CFLAGS += -c 

OBJS = $(wildcard *.o)

all:$(MSPU_TARGET)

ifeq ($(LIB_FORMAT), dynamic)
$(MSPU_TARGET):$(OBJS)
	$(LIB_GCC) $(OBJS) -o $@
	cp $@ $(LIB_DIR)
endif

ifeq ($(LIB_FORMAT), static)
$(MSPU_TARGET):$(OBJS)
	$(LIB_GCC) $@ $(OBJS)
	cp $@ $(LIB_DIR)
endif


clean:
	rm -rf $(OBJS) $(MSPU_TARGET) *.a *.so

