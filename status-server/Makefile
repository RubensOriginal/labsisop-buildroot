
CC = i686-linux-g++
PROG = TcpServer
CODE = main.cpp tcp_server.cpp
OBJECTS = $(CODE:.cpp=.o)

CPPFLAGS = -O3

UNAME = `uname`

all:
	$(CC) $(CPPFLAGS) -c $(CODE)
	$(CC) $(OBJECTS) $(CPPFLAGS) -o $(PROG)

clean:
	-@ rm -f $(OBJECTS) $(PROG)