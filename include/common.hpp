#ifndef NET_COMMON_HPP
#define NET_COMMON_HPP

#include <cstdint>
#include <cstdio>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/fcntl.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <format>

#define PORT 8080
#define MAX_EVENTS 1024
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10000

extern int tcp_listen_fd;
extern int udp_fd;
extern int epoll_fd;
extern volatile sig_atomic_t running;

extern long long total_clients;
extern int current_clients;

#endif // NET_COMMON_HPP

