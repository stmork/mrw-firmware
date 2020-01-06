CFLAGS	= -O3 -Wall
SRC		= canprog.c gwsim.c can_pc.c mrw_dump.c receive.c ping.c send.c\
	qryerr.c qrybuf.c reset.c switch.c signal.c form.c config.c setid.c\
	rail.c occupation.c light.c busy.c mrw_daemon.cc
PROGS		= canprog gwsim receive ping send reset switch signal form config qrybuf\
	qryerr setid rail occupation light busy mrw_daemon controller11

LDLIBS		= -lm -lpthread
CPPFLAGS	= -I ../mc

all:	$(PROGS)

canprog:	canprog.o can_pc.o mrw_dump.o

gwsim:	gwsim.o can_pc.o mrw_dump.o

receive:	receive.o can_pc.o mrw_dump.o

ping:	ping.o can_pc.o mrw_dump.o

send:	send.o can_pc.o mrw_dump.o

qrybuf:	qrybuf.o can_pc.o mrw_dump.o

qryerr:	qryerr.o can_pc.o mrw_dump.o

reset:	reset.o can_pc.o mrw_dump.o

switch:	switch.o can_pc.o mrw_dump.o

rail:	rail.o can_pc.o mrw_dump.o

light:	light.o can_pc.o mrw_dump.o

occupation:	occupation.o can_pc.o mrw_dump.o

signal:	signal.o can_pc.o mrw_dump.o

form:	form.o can_pc.o mrw_dump.o

config:	config.o can_pc.o mrw_dump.o

controller11:	controller11.o can_pc.o mrw_dump.o

setid:	setid.o can_pc.o mrw_dump.o

busy:	busy.o can_pc.o mrw_dump.o

mrw_daemon:	mrw_daemon.o mrw_dump.o can_pc.o
	$(LINK.cc) -o $@ $^ $(LDLIBS)

test:
	splint $(CPPFLAGS) $(SRC)

clean:
	rm -rf *.o $(PROGS) .dep

depend:
	$(CPP) $(CPPFLAGS) -M $(SRC) $(CFLAGS) >.dep

-include .dep
