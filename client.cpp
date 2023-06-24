#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <fstream>
#include <string>
#include <thread>

#include <cstring>
#include <regex>
using namespace std;
void writeToFile(int client_sock) {
    std::ofstream file;
    char filePath[1024];
    cin.getline(filePath,1024);
    file.open(filePath);
    char buffer[1024];
    int bytesReceived;
    while ((bytesReceived = recv(client_sock, buffer, sizeof(buffer), 0)) > 0) {
        file.write(buffer, bytesReceived);
    }
    file.close();
}
void createResultsFile(std::string str, int id) {
    std::replace(str.begin(), str.end(), ' ',',');
    size_t pos = str.rfind("Done.");
    if (pos != std::string::npos) {
        str.erase(pos);
    }
    string name = "results_client_" +  to_string(id) + ".csv";
    std::ofstream file(name);
    file << str;
    file.close();
}
void sendFile(char* path, int sock) {
    ifstream file(path);
    while (!file.is_open()) {
        char in[1024];
        cout << "can't open this file, write again the path" <<endl;
        cin.getline(in, sizeof(in));
        cout <<"in is: " <<in<<endl;
        file.open(in);
    }
    char buffer[1024];
    string line;
    while (getline(file, line)) {
        line += "\n";
        send(sock, line.c_str(), line.length(), 0);
        recv(sock,buffer, sizeof (buffer), 0);

    }
    send(sock, "$", 2, 0);
    file.close();
    recv(sock, buffer, sizeof(buffer), 0);
    string str(buffer);
    std::size_t found = str.find("!");
    if (found != std::string::npos) {

    } else {
        cout <<str<<endl;
    }
}
string recive_from_buffer(int socket) {
    string to_return = "";
    //cout<<"entering recieve from buffer"<<endl;
    while (1) {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        int read_bytes = recv(socket, buffer, sizeof(buffer), 0);
        if(read_bytes <= 0) {
            cout << "connection closed" <<endl;
            exit(1);
        }
        string bufferStr(buffer);

        bufferStr += "\n";

        std::size_t found = bufferStr.find("$");
        if (found != std::string::npos) {
            bufferStr.pop_back();
            bufferStr.pop_back();
            to_return += bufferStr;
            break;
        }
        to_return += bufferStr;
        send(socket, "?", 1, 0);
    }
    //cout<<"reciece from buffer returns "<<to_return<<"\n#########################\n"<<endl;
    return to_return;
}
string getChoice() {
    int c;
    while (true) {
        try{
            char input[1024];
            cin.getline(input, sizeof(input));
            c = atoi(input);
            if (c < 1 || (c>5 && c!=8)) {
                throw invalid_argument("not good");
            }
            if (c != atoi(input)) {
                throw invalid_argument("not good");
            }
            string s(input);
            return s;
        } catch (const invalid_argument& e) {
            cout << "choice must be in range 1-5 or 8" <<endl;
        }
    }
}
void createThread(int sock) {
    thread client_thread(writeToFile, sock);
    client_thread.detach();
}
int main(int argc, char** argv){
    char a[] = "$";
    int  sent;
    // IP address and port of the server to connect to
    const char* ip_address = argv[1];
    try {
        const int port_no = stoi(argv[2]);
        if (port_no != (int)(stod(argv[2])) || port_no < 1024||port_no>65535) {
            cout << "port number must be int in the correct range" << endl;
            exit(1);
        }
    }
    catch (const std::exception& ex) {
        cout << "error while trying to connect server and client" << endl;
        exit(1);
    }
    const int port_no = stoi(argv[2]);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        // If the return value is less than 0, an error occurred
        perror("error creating socket");
    }
    // Set up the address and port of the server to connect to
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin)); // Clear the memory
    sin.sin_family = AF_INET; // Use IPv4
    sin.sin_addr.s_addr = inet_addr(ip_address); // Convert the IP address string to a network byte order binary address
    sin.sin_port = htons(port_no); // Convert port to network byte order

    // Connect to the server
    if (connect(sock, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        // If the return value is less than 0, an error occurred
        perror("error connecting to server");
        exit(1);
    }
    char buf[1024];
    recv(sock, buf, sizeof (buf), 0);
    int clientId = atoi(buf);
    send(sock, a, sizeof(a), 0);
    string recived_message = recive_from_buffer(sock);
    cout<<recived_message<<endl;
    while(1) {
        char to_send[1024];
        string optionStr = getChoice();
        int option = stoi(optionStr);
        string st;
        int sent_bytes = send(sock, optionStr.c_str(), optionStr.length(), 0);
        if(sent_bytes <= 0) {
            cout << "connection closed" <<endl;
            exit(1);
        }
        switch (option) {
            case 1:
                recived_message = recive_from_buffer(sock); //upload message
                cout<<recived_message<<endl;
                cin.getline(to_send,1024); //get the path from user
                sendFile(to_send, sock);
                recived_message = recive_from_buffer(sock);
                cout<<recived_message<<endl;
                cin.getline(to_send,1024); //get the path from user
                sendFile(to_send, sock);
                break;
            case 2:
                recived_message = recive_from_buffer(sock);
                cout<<recived_message<<endl;
                memset(to_send, 0 , sizeof(to_send));
                cin.getline(to_send,1024); //get the path from user
                //st=to_send;
                //cout<<"msg = "<< to_send<<endl<<"size = "  << to_string(st.length()) <<endl;
                sent_bytes = send(sock, to_send, sizeof(to_send), 0);
                if (sent_bytes <= 0 ) {
                    cout << "connection closed" <<endl;
                    exit(1);
                }
                recived_message = recive_from_buffer(sock);
                cout << recived_message <<endl;
                sent = send(sock,to_send, sizeof(to_send), 0 );
                if(sent <= 0) {
                    cout << "connection closed" <<endl;
                    exit(1);
                }
                break;
            case 3:
                recived_message = recive_from_buffer(sock);
                cout<<recived_message<<endl;
                sent_bytes = send(sock, a, sizeof(a), 0);
                if(sent_bytes<=0){
                    cout<<"connectio closed"<<endl;
                    exit(1);
                }
                break;
            case 4:
                recived_message = recive_from_buffer(sock);
                cout<<recived_message<<endl;
                sent_bytes = send(sock, a, sizeof(a), 0);
                if(sent_bytes<=0){
                    cout<<"connectio closed"<<endl;
                    exit(1);
                }
                break;
            case 5:
                recived_message = recive_from_buffer(sock);
                createResultsFile(recived_message, clientId);
                createThread(sock);
                sent_bytes = send(sock, a, sizeof(a), 0);
                if(sent_bytes<=0){
                    cout<<"connectio closed"<<endl;
                    exit(1);
                }
                break;
            case 8:
                cout<<"closing client"<<endl;
                break;
            default:
                //cout<<"wrong option, write it again";
                break;
        }
        recived_message = recive_from_buffer(sock);
        cout<<recived_message<<endl;
    }
}