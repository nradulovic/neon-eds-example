AUTOMAKE_OPTIONS = foreign
SUBDIRS = neon .

CFLAGS += -Wall \
    -I$(top_srcdir)/bsp \
    -I$(top_srcdir)/neon/eds/include \
    -I$(top_srcdir)/neon/eds/port/x86-64-linux-gcc \
    -I$(top_srcdir)/neon/lib/include
                    
bin_PROGRAMS = blinky_fsm
blinky_fsm_SOURCES = \
    example/blinky_fsm/blinky_fsm.c

blinky_fsm_LDADD = ../neon/libneon.ls -lpthread
