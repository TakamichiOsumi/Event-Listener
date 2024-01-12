#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include "network_utils.h"

void
netutil_read_received_udp_msg(int udp_socket_fd, void *received_data,
			      size_t limit_length, bool *broken){
    char udp_buf[BUF_SIZE];
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);
    ssize_t recv_size;

    *broken = false;
    if ((recv_size = recvfrom(udp_socket_fd,
			      (char *)udp_buf, sizeof(udp_buf), 0,
			      (struct sockaddr *)&server_addr,
			      &addr_len)) < 0){
	fprintf(stderr, "Couldn't receive any data\n");
	exit(-1);
    }

    if (limit_length < recv_size){
	*broken = true;
	strncpy(received_data, udp_buf, limit_length);
    }else{
	strncpy(received_data, udp_buf, recv_size);
    }
}

int
netutil_create_udp_sock(int own_udp_port, in_addr_t own_node_ip){
    struct sockaddr_in udp_server_addr;
    int udp_fd;

    if ((udp_fd =
	 socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
	perror("socket");
	exit(-1);
    }

    udp_server_addr.sin_family = AF_INET;
    udp_server_addr.sin_port = htons(own_udp_port);
    udp_server_addr.sin_addr.s_addr = own_node_ip;

    if (bind(udp_fd, (struct sockaddr *)&udp_server_addr,
	     sizeof(udp_server_addr)) < 0){
	perror("bind");
	exit(-1);
    }

    return udp_fd;
}

void
netutil_send_udp_message(int udp_socket_fd, in_addr_t own_node_ip,
			 int target_udp_port, char *msg, size_t msg_size){
    struct sockaddr_in target_node;
    int udp_server = sizeof(struct sockaddr_in);

    /* The credential of the receiver */
    target_node.sin_family = AF_INET;
    target_node.sin_port = htons(target_udp_port);
    target_node.sin_addr.s_addr = own_node_ip;

    if (sendto(udp_socket_fd,
	       msg, msg_size, 0, (struct sockaddr *)&target_node, udp_server) < 0){
	perror("sendto");
	exit(-1);
    }else{
	printf("Sent the message '%s' over UDP to the node (port = %d)\n",
	       msg, target_udp_port);
    }
}
