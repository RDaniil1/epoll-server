#ifndef NETWORK_HPP
#define NETWORK_HPP

#include "common.hpp"

int set_nonblocking(int fd);
int create_and_bind(int domain, int type, int port);

void create_tcp_conn(struct epoll_event* event, struct sockaddr_in* client_addr, socklen_t* client_len);
void read_from_udp(char* buffer, struct sockaddr_in* client_addr, socklen_t* client_len);
void read_from_tcp(int i, struct epoll_event* events, char* buffer);

#endif // NETWORK_HPP
