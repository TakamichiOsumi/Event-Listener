#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include "network_utils.h"

#define FD_SIZE 32
static int target_fd_set[FD_SIZE];

void
init_fd_set(void){
    int i;

    for (i = 0; i < FD_SIZE; i++){
	target_fd_set[i] = -1;
    }
}

void
add_fd_set(int new_fd){
    int i;

    for (i = 0; i < FD_SIZE; i++){
	if (target_fd_set[i] == -1){
	    target_fd_set[i] = new_fd;
	    break;
	}
    }
}

int
get_max_fd(void){
    int i, max = -1;

    for (i = 0; i < FD_SIZE; i++){
	if (target_fd_set[i] > max)
	    max = target_fd_set[i];
    }

    return max;
}

void
re_init_readfds(fd_set *fdp){
    int i;

    FD_ZERO(fdp);
    for (i = 0; i < FD_SIZE; i++){
	if (target_fd_set[i] != -1){
	    FD_SET(target_fd_set[i], fdp);
	}
    }
}

int
main(int argc, char **argv){
    char *ip = "127.0.0.1", msg_buf[BUF_SIZE];
    int udp_port = 3000, udp_socket_fd;
    fd_set readfds;
    bool broken;

    udp_socket_fd = netutil_create_udp_sock(udp_port,
					    inet_addr(ip));

    /* for get_max_fd(), trace all the file descriptors */
    init_fd_set();
    add_fd_set(udp_socket_fd);
    
    FD_ZERO(&readfds);
    FD_SET(udp_socket_fd, &readfds);

    while(1){
	re_init_readfds(&readfds);

	printf("Will get blocked by select system call ...\n");
	if (select(get_max_fd() + 1, &readfds, NULL, NULL, NULL) < 0){
	    perror("select");
	    exit(-1);
	}

	netutil_read_received_udp_msg(udp_socket_fd,
				      msg_buf, BUF_SIZE, &broken);
	if (!broken)
	    printf("received message : '%s'\n", msg_buf);
    }

    return 0;
}
