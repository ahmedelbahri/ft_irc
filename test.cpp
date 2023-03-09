#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int server_fd, client_fd, max_fd, activity, valread;
    struct sockaddr_in address;
    char buffer[BUFFER_SIZE] = {0};
    std::vector<int> clients(MAX_CLIENTS, 0);
    fd_set read_fds;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8888);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return -1;
    }

    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listen failed" << std::endl;
        return -1;
    }

    std::cout << "Server listening on port 8888" << std::endl;

    while (true) {
        FD_ZERO(&read_fds);
        FD_SET(server_fd, &read_fds);
        max_fd = server_fd;

        for (int i = 0; i < MAX_CLIENTS; ++i) {
            client_fd = clients[i];
            if (client_fd > 0) {
                FD_SET(client_fd, &read_fds);
            }
            if (client_fd > max_fd) {
                max_fd = client_fd;
            }
        }

        activity = select(max_fd + 1, &read_fds, nullptr, nullptr, nullptr);

        if (activity < 0) {
            std::cerr << "Select error" << std::endl;
            continue;
        }

        if (FD_ISSET(server_fd, &read_fds)) {
            if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
                std::cerr << "Accept error" << std::endl;
                continue;
            }

            std::cout << "New client connected: " << client_fd << std::endl;

            for (int i = 0; i < MAX_CLIENTS; ++i) {
                if (clients[i] == 0) {
                    clients[i] = client_fd;
                    break;
                }
            }
        }

        for (int i = 0; i < MAX_CLIENTS; ++i) {
            client_fd = clients[i];
            if (FD_ISSET(client_fd, &read_fds)) {
                valread = read(client_fd, buffer, BUFFER_SIZE);
                if (valread == 0) {
                    std::cout << "Client disconnected: " << client_fd << std::endl;
                    close(client_fd);
                    clients[i] = 0;
                } else {
                    std::cout << "Received message from client " << client_fd << ": " << buffer << std::endl;
                    // Process message
                    // ...
                }
            }
        }
    }

    return 0;
}
