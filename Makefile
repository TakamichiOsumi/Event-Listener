CC	= gcc
CFLAGS	= -O0 -Wall
PROGRAM	= event_listener

all: $(PROGRAM) udp_server udp_client

$(PROGRAM): network_utils.o
	$(CC) $(CFLAGS) event_listener.c $< -o $@

network_utils.o: network_utils.c
	$(CC) $(CFLAGS) -c network_utils.c

udp_server: network_utils.o
	$(CC) $(CFLAGS) udp_server.c $< -o $@

udp_client: network_utils.o
	$(CC) $(CFLAGS) udp_client.c $< -o $@

.PHONY:clean

clean:
	@rm -f *.o $(PROGRAM) udp_server udp_client
