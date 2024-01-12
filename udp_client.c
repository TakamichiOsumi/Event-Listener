#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "network_utils.h"
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "network_utils.h"

int
main(void){
    char *ip = "127.0.0.1",
	*msg = "Hello udp server";
    int udp_sock_fd;

    if ((udp_sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
	perror("socket");
	exit(-1);
    }

    printf("Will send messages.\n");
    netutil_send_udp_message(udp_sock_fd, inet_addr(ip),
			     3000, msg, strlen(msg));
    netutil_send_udp_message(udp_sock_fd, inet_addr(ip),
			     3001, msg, strlen(msg));
    close(udp_sock_fd);
    printf("Done.\n");
}
