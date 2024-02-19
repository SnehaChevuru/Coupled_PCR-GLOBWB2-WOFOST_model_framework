SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.o=%.c)
#OBJS    = afgen.o assim.o astro.o cropdata.o develop.o fillvar.o lai.o lai2.o leaves.o limit.o max.o min.o meteodata.o penman.o sitedata.o soildata.o watfd.o wofost.o management.o nutrients.o clean.o evtra.o
EXECUTABLE = wofost
CC       = gcc
CFLAGS  = -g -ggdb -O0 -Wall -Wextra -std=c99 -lm -lnetcdf \
#CFLAGS   = -Wall -O3

all: $(EXECUTABLE)

$(EXECUTABLE) : $(OBJS)
    $(CC) -o $(EXECUTABLE) $(OBJS) -lm $(CFLAGS)


%.o: %.c
    $(CC) $(CFLAGS) -c -o $@ $<

TAGS:   $(SRCS)
    etags $(SRCS)
clean::
    \rm -f TAGS $(EXECUTABLE)
