CC		= gcc
CPP		= gcc
SRC		= ubbr.c uart.c mcp_config.c rate.c
PROGS		= ubbr uart mcp_config rate
LDFLAGS		= -lm
CPPFLAGS	= -I ../mc

all:	$(PROGS)

clean:
	rm -rf *.o $(PROGS) .dep

depend:
	$(CPP) $(CPPFLAGS) -M $(SRC) $(CFLAGS) >.dep

-include .dep
