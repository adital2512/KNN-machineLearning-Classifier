#include "HandleClient.h"
HandleClient::HandleClient(int socket, int clientIndex) {
    this->clientIndex = clientIndex;
    //creating vector made of command objects, using command design pattern
    (this->commands).push_back(nullptr); 
    (this->commands).push_back(new choice1());
    (this->commands).push_back(new choice2());
    (this->commands).push_back(new choice3());
    (this->commands).push_back(new choice4());
    (this->commands).push_back(new choice5());
    (this->commands).push_back(nullptr);
    (this->commands).push_back(nullptr);
    (this->commands).push_back(new choice8());
    this->client_sock = socket;
    this->k = 5;
    this->hasFiles = false;
    this->distanceMethodStr = "AUC";
    this->hasResults=false;
    this->pathClassified = "classified_" + to_string(this->getClientId()) + ".csv";
    this->pathUnclassified = "Unclassified_" + to_string(this->getClientId()) + ".csv";
    this->pathToResults="results_"+to_string(this->getClientId())+".csv";
}
void HandleClient::run() {
    string not_int = "choice must be int in range 1-5 or 8\n$";
    char menu[] = "Welcome to the KNN Classifier Server. Please choose an option:\n1. upload an unclassified csv data file\n2. algorithm settings\n3. classify data\n4. display results\n5. download results\n8. exit\n$";
    int choice = -1;
    while (true) {
        char buffer[4096];
        memset(buffer, 0, sizeof(buffer));
        int expected_data_len = sizeof(buffer);
        int sent_bytes = send(client_sock, menu, sizeof(menu), 0);
        if (sent_bytes <= 0) {
        }
        int read_bytes = recv(client_sock, buffer, expected_data_len, 0);
        if (read_bytes == 0) {
            //connection is closed
            break;
        } else if (read_bytes < 0) {
            cout << "error reading data" << endl;
            return;
        } else {
            //everything works - call the right choice
            buffer[read_bytes] = '\0'; //make it readable
            try {
                choice = stoi(buffer);
            } catch (const exception &ex) {
                int sent_bytes = send(client_sock, not_int.c_str(), sizeof(not_int), 0);
                if (sent_bytes < 0) {
                    perror("error sending to client");
                    return;
                }
            }
            if (!(choice < 1 || (choice > 5 && choice != 8))) {
                ((this->commands)[choice])->execute(this);
                if(choice == 8) {
                    break;
                }
            } else {
                sent_bytes = send(client_sock, not_int.c_str(), sizeof(not_int), 0);
                if (sent_bytes < 0) {
                    perror("error sending to client");
                    return;
                }
            }
        }
    }
}
HandleClient::~HandleClient() {
    for(int i = 0 ;i<int(this->commands.size());i++) {
        if ((this->commands)[i] != nullptr) {
            delete (this->commands)[i];
        }
    }
    close(client_sock);
}
int HandleClient::getK() {
    return this->k;
}
string HandleClient::getDistanceMethod() {
    return this->distanceMethodStr;
}
void HandleClient::setBool() {
    this->hasFiles = true;
}
string HandleClient::getClassifyPath() {
    return this->pathClassified;
}
string HandleClient::getUnClassifyPath() {
    return this->pathUnclassified;
}
bool HandleClient::hasFilesBool() {
    return this->hasFiles;
}
void HandleClient::setK(int k) {
    this->k = k;
}
void HandleClient::setDistanceMethod(std::string dm) {
    this->distanceMethodStr = dm;
}
int HandleClient::getSocket() {
    return this->client_sock;
}
int HandleClient::getClientId() {
    return this->clientIndex;
}
string HandleClient::getPathToResults() {
    return this->pathToResults;
}
bool HandleClient::hasResultsBool() {
    return this->hasResults;
}
void HandleClient::setResultsBool() {
    this->hasResults = true;
}
