#ifndef UTILS_HPP
#define UTILS_HPP

#include "common.hpp"

void get_current_time(char* buf, size_t bufsize);

void send_current_time(char* timebuf, unsigned long timebuf_size, 
                        char* response, unsigned long response_size, 
                        int client_fd, const struct sockaddr_in* client_addr);
void send_stats(char* response, unsigned long response_size, int client_fd, 
                const struct sockaddr_in* client_addr);
void stop_server(char* response, unsigned long response_size, int client_fd);

void handle_command(const char* msg, int client_fd, const struct sockaddr_in* client_addr);
void process_message(const char* msg, size_t len, int client_fd, struct sockaddr_in* client_addr);

#endif // UTILS_HPP
