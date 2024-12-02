#ifndef NETWORK_H
#define NETWORK_H

int connect_to_server(const char *ip, int port);
void send_to_server(int sock, const char *message);
void receive_from_server(int sock, char *buffer, int size);

#endif // NETWORK_H
