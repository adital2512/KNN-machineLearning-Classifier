#include <iostream>
#include "HandleClient.h"
#include <thread>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
using namespace std;
void threadedClient(int client_socket, int clientIndex) {
    HandleClient hc(client_socket, clientIndex);
    string s = to_string(clientIndex);
    send(hc.getSocket(), s.c_str(), s.length(), 0);
    char buf[1024];
    recv(hc.getSocket(), buf, sizeof(buf),0);
    hc.run();
}

int main(int argc, char* argv[]) {
    string result;
    int port;
    int clientIndex = 0;
    try {
        port = stoi(argv[1]);
        if (port != (int) (stod(argv[1])) || port < 1024 || port > 65535) {
            cout << "port not in range" << endl;
            exit(1);
        }

    }
    catch (const std::exception &ex) {
        cout << "port not in range" << endl;
        exit(1);
    }
    const int server_port = port;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("error creating socket");
        exit(1);
    }
    struct sockaddr_in sin;

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(server_port);
    if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("error binding socket");
        exit(1);
    }
    if (listen(sock, 5) < 0) {
        perror("error listening to a socket");
        exit(1);
    }
    while (1) {
        struct sockaddr_in client_sin;
        unsigned int addr_len = sizeof(client_sin);
        int client_sock = accept(sock, (struct sockaddr *) &client_sin, &addr_len);
        if (client_sock < 0) {
            perror("error accepting client");
            exit(1);
        }
        thread client_thread(threadedClient, client_sock, clientIndex++);
        client_thread.detach();
    }
}