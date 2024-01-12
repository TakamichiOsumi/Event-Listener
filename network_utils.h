#ifndef ___NETWORK_UTILS__
#define ___NETWORK_UTILS__
#include <stdbool.h>
#include <netinet/in.h>

#define BUF_SIZE 1024

void netutil_read_received_udp_msg(int udp_socket_fd,
				   void *received_data,
				   size_t data_size,
				   bool *broken);
int netutil_create_udp_sock(int own_upd_port,
			    in_addr_t own_node_ip);
void
netutil_send_udp_message(int udp_socket_fd,
			 in_addr_t own_node_ip,
			 int target_udp_port,
			 char *msg, size_t msg_size);

#endif
