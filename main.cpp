#include "common.hpp"
#include "utils.hpp"
#include "network.hpp"

int tcp_listen_fd = -1;
int udp_fd = -1;
int epoll_fd = -1;
volatile sig_atomic_t running = 1;

long long total_clients = 0;
int current_clients = 0;


int main() {
    struct epoll_event event, events[MAX_EVENTS];
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    tcp_listen_fd = create_and_bind(AF_INET, SOCK_STREAM, PORT);
    udp_fd = create_and_bind(AF_INET, SOCK_DGRAM, PORT);

    if (tcp_listen_fd < 0 || udp_fd < 0) {
        return 1;
    }

    set_nonblocking(tcp_listen_fd);
    set_nonblocking(udp_fd);

    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) {
        return 1;
    }

    event.events = EPOLLIN;
    event.data.fd = tcp_listen_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, tcp_listen_fd, &event);

    event.data.fd = udp_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, udp_fd, &event);

    printf("Server started on port %d\n", PORT);

    while (running) {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, 1000);

        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == tcp_listen_fd) {
                create_tcp_conn(&event, &client_addr, &client_len);
            } else if (events[i].data.fd == udp_fd) {
                read_from_udp(buffer, &client_addr, &client_len);
            } else {
                read_from_tcp(i, events, buffer);
            }
        }
    }

    close(tcp_listen_fd);
    close(udp_fd);
    close(epoll_fd);
    printf("Server stopped\n");
    return 0;
}
