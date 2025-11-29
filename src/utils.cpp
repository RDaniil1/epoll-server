#include "utils.hpp"
#include <cstdio>

void get_current_time(char* buf, size_t bufsize) {
    struct tm tm;
    time_t t = time(NULL);
    localtime_r(&t, &tm);
    strftime(buf, bufsize, "%Y-%m-%d %H:%M:%S", &tm);
}

void send_current_time(char* timebuf, unsigned long timebuf_size, char* response, unsigned long response_size, int client_fd, const struct sockaddr_in* client_addr) {
    get_current_time(timebuf, timebuf_size);
    snprintf(response, response_size, "Current time: %s\n", timebuf);
    if (client_fd >= 0) {
        send(client_fd, response, response_size - 1, 0);
    } else {
        printf("UDP %s:%d: %s", inet_ntoa(client_addr->sin_addr), 
                ntohs(client_addr->sin_port), response);
    }
}

void send_stats(char* response, unsigned long response_size, int client_fd, const struct sockaddr_in* client_addr) {
    snprintf(response, response_size, "Total clients: %lld, Current: %d\n", 
                total_clients, current_clients);
    if (client_fd >= 0) {
        send(client_fd, response, response_size - 1, 0);
    } else {
        printf("UDP %s:%d: %s", inet_ntoa(client_addr->sin_addr), 
                ntohs(client_addr->sin_port), response);
    }
}

void stop_server(char* response, unsigned long response_size, int client_fd) {
    snprintf(response, response_size, "Server shutting down...\n");
    if (client_fd >= 0) {
        send(client_fd, response, response_size - 1, 0);
    }
    running = 0;
}

void handle_command(const char* msg, int client_fd, const struct sockaddr_in* client_addr) {
    char timebuf[32];
    char response[BUFFER_SIZE];

    if (strcmp(msg, "/time") == 0) {
        send_current_time(timebuf, sizeof(timebuf), response, sizeof(response), client_fd, client_addr);
    } else if (strcmp(msg, "/stats") == 0) {
        send_stats(response, sizeof(response), client_fd, client_addr);
    } else if (strcmp(msg, "/shutdown") == 0) {
        stop_server(response, sizeof(response), client_fd);
    }
}

void process_message(const char* msg, size_t len, int client_fd, struct sockaddr_in* client_addr) {
    if (len == 0 || msg[0] != '/') {
        if (client_fd >= 0) {
            send(client_fd, msg, len, 0);
        } else {
            sendto(udp_fd, msg, len, 0, (struct sockaddr*)client_addr, sizeof(*client_addr));
        }
        return;
    }

    char cmd[64];
    size_t cmd_len = 0;
    while (cmd_len < len && cmd_len < sizeof(cmd) - 1 && msg[cmd_len] != ' ') {
        cmd[cmd_len] = msg[cmd_len];
        cmd_len++;
    }
    cmd[cmd_len] = '\0';

    handle_command(cmd, client_fd, client_addr);
}
