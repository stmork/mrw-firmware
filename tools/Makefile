CC		= gcc
CPP		= gcc
SRC		= ubbr.c uart.c mcp_config.c rate.c table.c
PROGS		= ubbr uart mcp_config rate table
CFLAGS		= -O3
LDLIBS		= -lm
CPPFLAGS	= -I ../mc

all:	$(PROGS)

clean:
	rm -rf *.o $(PROGS) .dep

depend:
	$(CPP) $(CPPFLAGS) -M $(SRC) $(CFLAGS) >.dep

-include .dep
