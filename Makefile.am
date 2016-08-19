AUTOMAKE_OPTIONS = foreign
SUBDIRS = neon .

CFLAGS += -Wall -O0 -g3 \
    -I$(top_srcdir)/common \
    -I$(top_srcdir)/neon/eds/include \
    -I$(top_srcdir)/neon/eds/port/x86-64-linux-gcc \
    -I$(top_srcdir)/neon/lib/include
                    
bin_PROGRAMS = blinky_fsm heart_beat_dyn_fsm
blinky_fsm_SOURCES = \
    example/blinky_fsm/blinky_fsm.c

blinky_fsm_LDADD = ./neon/libneon.la -lpthread

heart_beat_dyn_fsm_SOURCES = \
    example/heart_beat_dyn_fsm/heart_beat_dyn_fsm.c

heart_beat_dyn_fsm_LDADD = ./neon/libneon.la -lpthread
