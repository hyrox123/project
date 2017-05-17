# Makefile
#
ROOTDIR = ../..
include $(ROOTDIR)/Rules.make 

CFLAGS += -c 

SRC  = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRC))

all:$(OBJS)

%.o:%.c
	$(GCC) $(CFLAGS) -I$(OS_INCLUDES) -I$(GLOBEL_INCLUDES) $< -o $@
	mv $@ $(OUTPUT_PATH) 

clean:
	rm -rf $(OBJS) $(OS_TARGET) *.a *.so

