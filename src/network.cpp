#include "network.hpp"
#include "utils.hpp"

int create_and_bind(int domain, int type, int port) {
    int fd = socket(domain, type, 0);
    if (fd < 0) {
        return -1;
    }

    int opt = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = (sa_family_t)domain;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons((uint16_t)port);

    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        close(fd);
        return -1;
    }

    if (type == SOCK_STREAM && listen(fd, SOMAXCONN) < 0) {
        close(fd);
        return -1;
    }

    return fd;
}

int set_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        return -1;
    }
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void create_tcp_conn(struct epoll_event* event, struct sockaddr_in* client_addr, socklen_t* client_len) {
    int client_fd = accept(tcp_listen_fd, (struct sockaddr*)client_addr, client_len);
    if (client_fd >= 0) {
        total_clients++;
        current_clients++;

        if (set_nonblocking(client_fd) == 0) {
            event->events = EPOLLIN | EPOLLET;
            event->data.fd = client_fd;
            epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, event);
        } else {
            close(client_fd);
            current_clients--;
        }
    }
}


void read_from_udp(char* buffer, struct sockaddr_in* client_addr, socklen_t* client_len) {
    ssize_t len = recvfrom(udp_fd, buffer, BUFFER_SIZE - 1, 0,
                          (struct sockaddr*)client_addr, client_len);
    if (len > 0) {
        buffer[len] = '\0';
        process_message(buffer, len, -1, client_addr);
    }
}

void read_from_tcp(int i, struct epoll_event* events, char* buffer) {
    int client_fd = events[i].data.fd;
    ssize_t len = read(client_fd, buffer, BUFFER_SIZE - 1);

    if (len > 0) {
        buffer[len] = '\0';
        process_message(buffer, len, client_fd, NULL);
    } else {
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, NULL);
        close(client_fd);
        current_clients--;
    }
}

