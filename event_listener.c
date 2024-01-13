#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include "network_utils.h"

#define BUFFER_SIZE 1024

typedef void (*udp_react_function_cb)(char *msg,
				      uint32_t msg_size,
				      char *sender_ip,
				      uint32_t port_no);

typedef struct udp_listener_args {
    char ip_addr[16];
    uint32_t port_no;
    udp_react_function_cb func_cb;
} udp_listener_args;

static void
udp_react_fn(char *msg, uint32_t msg_size,
	     char *sender_ip, uint32_t port_no){
    printf("%s() : msg = '%s', msg size = %u\n",
	   __FUNCTION__, msg, msg_size);
}

static void *
udp_listener_main(void *args){
    udp_listener_args *ulargs = args;
    int udp_sock_fd;
    fd_set readfds;
    bool broken;
    char msg_buf[BUFFER_SIZE];

    udp_sock_fd = netutil_create_udp_sock(ulargs->port_no,
					  inet_addr(ulargs->ip_addr));
    FD_ZERO(&readfds);
    FD_SET(udp_sock_fd, &readfds);

    while(1){
	printf("Will get blocked by select system call\n");
	if (select(udp_sock_fd + 1, &readfds, NULL, NULL, NULL) < 0){
	    perror("select");
	    exit(-1);
	}

	netutil_read_received_udp_msg(udp_sock_fd,
				      msg_buf, BUFFER_SIZE, &broken);
	if (!broken){
	    printf("Received message : '%s'\n", msg_buf);

	    if (ulargs->func_cb)
		ulargs->func_cb(msg_buf, strlen(msg_buf),
				ulargs->ip_addr, ulargs->port_no);
	}
    }

    return NULL;
}

static pthread_t *
udp_server_thread_launch(char *ip, int port_no,
			 udp_react_function_cb func_cb){
    pthread_attr_t attr;
    pthread_t *udp_listener_thread;
    udp_listener_args *args;

    if ((udp_listener_thread = (pthread_t *)malloc(sizeof(pthread_t))) == NULL){
	perror("malloc");
	exit(-1);
    }

    if ((args = (udp_listener_args *)malloc(sizeof(udp_listener_args))) == NULL){
	perror("malloc");
	exit(-1);
    }

    strncpy(args->ip_addr, ip, sizeof(args->ip_addr));
    args->port_no = port_no;
    args->func_cb = func_cb;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(udp_listener_thread,
		   &attr, udp_listener_main, (void *) args);

    return udp_listener_thread;
}

int
main(int argc, char **argv){
    pthread_t *listener1, *listener2;

    listener1 = udp_server_thread_launch("127.0.0.1", 3000,
					 udp_react_fn);
    listener2 = udp_server_thread_launch("127.0.0.1", 3001,
					 udp_react_fn);
    pthread_exit(0);

    return 0;
}
