#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
using namespace std;
int main(int argc, char* argv[]) {
    string result;
    int port;
	try {
		port = stoi(argv[2]);
		if (port != (int)(stod(argv[2])) || port < 1024|| port>65535) {
			cout << "port not in range" << endl;
			exit(1);
		}

	}
	catch (const std::exception& ex) {
		cout << "port not in range" << endl;
		exit(1);
	}
        //get the path from args
        string path = argv[1];

        const int server_port= port ;
        int sock= socket(AF_INET, SOCK_STREAM,0);
        if (sock < 0)
        {
            perror("error creating socket");
            exit(1);
        }
        struct sockaddr_in sin;
        
        memset(&sin,0, sizeof(sin));
        sin.sin_family= AF_INET;
        sin.sin_addr.s_addr= INADDR_ANY;
        sin.sin_port= htons(server_port);
        if (bind(sock,(struct sockaddr*)&sin, sizeof(sin))<0)
        {
             perror("error binding socket");
             exit(1);
        }
        if (listen(sock,5) < 0){
        perror("error listening to a socket");
        exit(1);
        }
        while(1) {
            struct sockaddr_in client_sin;
            unsigned int addr_len= sizeof(client_sin);
            int client_sock= accept(sock,(struct sockaddr*)&client_sin,&addr_len);
            if (client_sock < 0) {
            perror("error accepting client");
            exit(1);
            }
            while (1) {
                char buffer[4096];
                memset(buffer, 0, sizeof(buffer));
                int expected_data_len = sizeof(buffer);
                int read_bytes= recv(client_sock, buffer, expected_data_len,0);
                if (read_bytes == 0) {
                    //connection is closed
                    break;
                }
                else if (read_bytes < 0)
                {
                    cout<<"error reading data"<<endl;
                    exit(1);
                }
                else if(strcmp("-1", buffer) == 0){
                    break;
                }
                else
                {
                    result = handleFullInput(buffer,sizeof(buffer) / sizeof(char), path);
                }
                int sent_bytes= send(client_sock, result.c_str(), sizeof(result),0);
                if (sent_bytes < 0)
                {
                    perror("error sending to client");
                    exit(1);
                }
            }
        }
    close(sock);
    return 0;
}