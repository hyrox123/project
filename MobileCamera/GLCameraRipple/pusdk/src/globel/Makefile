# Makefile
#
ROOTDIR = ../..
include $(ROOTDIR)/Rules.make 

CFLAGS += -c -fPIC 
INCLUDES = -I$(OS_INCLUDES) -I$(MSPU_INCLUDES) -I$(GBPU_INCLUDES) -I$(GLOBEL_INCLUDES)

SRC  = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRC))

all:$(OBJS)

%.o:%.c
	$(GCC) $(CFLAGS) $(INCLUDES) $< -o $@
	mv $@  $(OUTPUT_PATH)

clean:
	rm -rf $(OBJS) 
