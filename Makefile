CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lncurses -lpanel -lpthread

# Client objects
CLIENT_OBJS = main.o ui.o auth.o channels.o messaging.o users.o network.o

# Server objects
SERVER_OBJS = server.o server_handlers.o server_main.o

all: my_dispute my_dispute_server

# Client executable
my_dispute: $(CLIENT_OBJS)
	$(CC) -o $@ $(CLIENT_OBJS) $(LDFLAGS)

# Server executable
my_dispute_server: $(SERVER_OBJS)
	$(CC) -o $@ $(SERVER_OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o my_dispute my_dispute_server

.PHONY: all clean 