
CC = i686-linux-g++
# CC = g++
PROG = TcpServer

CODE = main.cpp tcp_server.cpp os_status.cpp
OBJECTS = $(CODE:.cpp=.o)

CPPFLAGS = -O3

UNAME = `uname`

all: $(TARGET)
	-@make $(UNAME)

%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@
	
Linux: $(OBJECTS)
	$(CC) $(OBJECTS) $(CPPFLAGS) -o $(PROG)

clean:
	-@ rm -f $(OBJECTS) $(PROG)