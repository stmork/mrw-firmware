CC	= gcc
CPP	= gcc
SRC	= mrw_daemon.cc SerialLine.cc ServerSocket.cc UartReader.cc Log.cc ClientHandler.cc
PROGS	= mrw_daemon

LDFLAGS		= -lm -lpthread
CPPFLAGS	= -I ../mc

all:	$(PROGS)

mrw_daemon:	mrw_daemon.o SerialLine.o ServerSocket.o UartReader.o Log.o ClientHandler.o
	$(LINK.cc) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf *.o $(PROGS) .dep

depend:
	$(CPP) $(CPPFLAGS) -M $(SRC) $(CFLAGS) >.dep

-include .dep
