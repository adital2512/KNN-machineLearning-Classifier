#ifndef THREADS_PROJECT_HANDLECLIENT_H
#define THREADS_PROJECT_HANDLECLIENT_H

#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include "classifier.h"
#include <thread>
#include <cstring>
#include <sys/socket.h>
#include <iostream>
#include <regex>
class HandleClient {
public:
    HandleClient(int client_socket, int clientIndex);
    ~HandleClient();
    void run();
    void setBool();
    bool hasFilesBool();
    void setK(int k);
    void setDistanceMethod(string dm);
    int getSocket();
    int getClientId();
    int getK();
    string getDistanceMethod();
    string getClassifyPath();
    string getUnClassifyPath();
    string getPathToResults();
    void setResultsBool();
    bool hasResultsBool();
private:
    bool hasResults;

    int clientIndex;
    bool hasFiles = false;
    vector<struct command*> commands;
    int client_sock;
    string pathUnclassified;
    string pathClassified;
    string pathToResults;
    string distanceMethodStr;
    int k;
};
class command {
public:
    virtual ~command() {};
    virtual void execute(HandleClient* hc) = 0;
};
class choice1 : public command {
public :
    void execute(HandleClient* hc) override {
    int sent, rec;
        {
            //cout << "creating file 1 - " + hc->getClassifyPath() << endl;
            char message[] = "Please upload your local train CSV file.$";
            sent = send(hc->getSocket(), message, sizeof(message), 0);
            if(sent <= 0) {
                cout << "connection closed" << endl;
                return;
            }
            ofstream output_file(hc->getClassifyPath());

            while (1) {
                char buffer[1024];
                memset(buffer, 0, sizeof(buffer));
                rec = recv(hc->getSocket(), buffer, sizeof(buffer), 0);
                if (rec <= 0) {
                    cout <<"connection closed" <<endl;
                    return;
                }
                string bufferStr(buffer);
                bufferStr.erase(std::remove_if(bufferStr.begin(), bufferStr.end(), ::isspace),
                                bufferStr.end());
                bufferStr += "\n";

                std::size_t found = bufferStr.find("$");
                if (found != std::string::npos) {
                    break;
                }
                output_file << bufferStr;
                sent = send(hc->getSocket(), "$", 1, 0);
                if(sent <= 0) {
                    cout << "connection closed" << endl;
                    return;
                }
            }
            output_file.close();
        }
        {
            char message3[] = "!";
            sent= send(hc->getSocket(), message3, sizeof(message3), 0);
            if(sent <= 0) {
                cout << "connection closed" << endl;
                return;
            }
            char message1[] = "Upload complete.\nPlease upload your local test CSV file.$";
            sent = send(hc->getSocket(), message1, sizeof(message1), 0);
            if (sent <= 0) {
                cout << "connection closed" << endl;
                return;
            }
            ofstream output_file1(hc->getUnClassifyPath());

            while (1) {
                char buffer1[1024];
                memset(buffer1, 0, sizeof(buffer1));
                rec = recv(hc->getSocket(), buffer1, sizeof(buffer1), 0);
                if (rec <= 0) {
                    cout <<"connection closed" <<endl;
                    return;
                }
                string bufferStr1(buffer1);
                bufferStr1.erase(std::remove_if(bufferStr1.begin(), bufferStr1.end(), ::isspace),
                                 bufferStr1.end());
                bufferStr1 += "\n";

                std::size_t found1 = bufferStr1.find("$");
                if (found1 != std::string::npos) {
                    break;
                }
                output_file1 << bufferStr1;
                sent = send(hc->getSocket(), "$", 1, 0);
                if(sent <= 0) {
                    cout << "connection closed" << endl;
                    return;
                }
            }
            char message2[] = "Upload complete";
            sent = send(hc->getSocket(), message2, sizeof(message2), 0);
            if(sent <= 0) {
                cout << "connection closed" << endl;
                return;
            }
            output_file1.close();
        }
        hc->setBool();
    }
};
class choice2 : public command {
public :
    void execute(HandleClient* hc) override {
    int sent, rec;
        int k = hc->getK();
        string distanceMethod = hc->getDistanceMethod();
        string message = "The current KNN parameters are: K = " + to_string(k) + ", distance metric = " + distanceMethod + "$";
        char messageArray[message.size() + 1];
        strcpy(messageArray, message.c_str());
        char buffer[1024];
        sent = send(hc->getSocket(), messageArray,sizeof(messageArray), 0);
        if(sent <= 0) {
            cout << "connection closed" << endl;
            return;
        }
        memset(buffer, 0, sizeof(buffer));
        rec = recv(hc->getSocket(), buffer, sizeof(buffer), 0);
        string s(buffer);
        ///cout << "length is" << s.length() << "s is: " << s <<endl;
        if (rec <= 0) {
            cout <<"connection closed" <<endl;
            return;
        } else if (s.length() == 0) {
            char d[] = "$";
            sent = send(hc->getSocket(), d, sizeof(d), 0);
            if (sent <=0 ) {
                cout <<"connection closed"<<endl;
                return;
            }
            rec = recv(hc->getSocket(), buffer, sizeof(buffer), 0);
            if (rec <=0 ) {
                cout <<"connection closed"<<endl;
                return;
            }
        }else {
            int x;
            char str[4];
            char error[1024];
            try {
                int result = sscanf(buffer, "%d %s", &x, str);
                if (result != 2) {
                    throw std::invalid_argument("Invalid format$");
                }
                if (x <= 0) {
                    throw std::invalid_argument("invalid value for K$");
                }
                if (strcmp(str, "MAN") != 0 && strcmp(str, "AUC") != 0 && strcmp(str, "CHB") != 0 &&
                    strcmp(str, "CAN") != 0 && strcmp(str, "MIN") != 0) {
                    throw std::invalid_argument("invalid value for metric$");
                }
                string sm(str);
                hc->setK(x);
                hc->setDistanceMethod(sm);
                char d[] = "$";
                sent = send(hc->getSocket(), d, sizeof(d), 0);
                if (sent <=0 ) {
                    cout <<"connection closed"<<endl;
                    return;
                }
                rec = recv(hc->getSocket(), buffer, sizeof(buffer), 0);
                if (rec <=0 ) {
                    cout <<"connection closed"<<endl;
                    return;
                }
            } catch (const std::invalid_argument &e) {
                strncpy(error, e.what(), sizeof(error));
                error[sizeof(error) - 1] = 0;
               // cout << "error is: " << error << endl;
                sent = send(hc->getSocket(), error, sizeof(error), 0);
                if (sent <=0 ) {
                    cout <<"connection closed"<<endl;
                    return;
                }
                rec = recv(hc->getSocket(), buffer, sizeof(buffer), 0);
                if (rec <=0 ) {
                    cout <<"connection closed"<<endl;
                    return;
                }
            }
        }
    }
};
class choice3 : public command {
public :
    void execute(HandleClient* hc) override {
        //check if the user upload 2 files, if not can not perform this commad ans sends him a messeage
        char buff [1024];
        if(!hc->hasFilesBool()){
            char msgStr [] = "please upload data\n$";
            send(hc->getSocket(),msgStr, sizeof(msgStr), 0 );
            recv(hc->getSocket(),buff,sizeof(buff),0);
            return;

        }
        string pathResults=hc->getPathToResults();
        //cout<<"path to results is--"<<pathResults<<endl;
        std::ofstream outfile(pathResults, std::ofstream::trunc);
        outfile.close();
        classifier c;
        HandleFile hf;
        int count = 0;
        string fileName = hc->getUnClassifyPath();
        ifstream MyFile;
        try {
            MyFile.open(fileName);
        }
        catch (const std::exception &ex) {
            cout << "Wrong path to file! there is no a file in this path";
            //return "invalid input!";
        }
        while (MyFile.good()) {
            vec v;
            count++;
            string line;
            getline(MyFile, line);
            std::replace(line.begin(), line.end(), ',', ' ');//here i change all commas to space
            string type;
            string path = hc->getClassifyPath();
            string t=c.fillVector(line,v);
            //cout<<line<<endl;
            type=c.handleInput(line,hc->getDistanceMethod(),hc->getK(),hc->getClassifyPath());
            if(type!="invalid input!") {
                this->writeFile(v, type,pathResults);
            }
        }
        if (count == 0) {//that means i read 0 lines from the file
            //return "invalid input!";
        }
        hc->setResultsBool();
        string msgStr = "complete data classifying\n$";
        send(hc->getSocket(),msgStr.c_str(), msgStr.length(), 0 );
        recv(hc->getSocket(),buff,sizeof(buff),0);
        return;
    }
    void writeFile(vec v, string type, string path){

        ofstream file;
        file.open(path,ios::app);

        for (int i = 0; i < int(v.size()); i++) {
            file << v[i];
            file << ",";
            //   }
        }
        file<<type;
        file << endl;
        file.close();
    }

};
class choice4 : public command {
public :
    void execute(HandleClient* hc) override {
        char buff[1024];
        //cout<<"has result value is"<<hc->hasResultsBool()<<"the files value is"<<hc->hasFilesBool()<<endl;
        if(!((hc->hasResultsBool())&&((hc->hasFilesBool())))){
            string msgStr = "please upload data, and then calculate the result\n$";
            send(hc->getSocket(),msgStr.c_str(), msgStr.length(), 0 );
            recv(hc->getSocket(),buff,sizeof(buff),0);
            return;
        }
        //cout<<"the value of bool is"<<hc->hasResultsBool()<<endl;
        if(!hc->hasResultsBool()){
            //cout<<"i am here!!!!!!!"<<endl;
            string msgStr = "please calculate results\n$";
            send(hc->getSocket(),msgStr.c_str(), msgStr.length(), 0 );
            return;
        }

        HandleFile hf;
        string fileName = hc->getPathToResults();
        int counter = 0;
        ifstream MyFile;
        try {
            MyFile.open(fileName);
        }
        catch (const std::exception &ex) {
            cout << "Wrong path to file! there is no a file in this path";
            //return "invalid input!";
        }//sendning back to client
        while (MyFile.good()) {
            counter++;
            string line;
            getline(MyFile, line);
            size_t lastCommaIndex = line.find_last_of(',');
            string type = line.substr(lastCommaIndex + 1);
            if(type==""){
                char msg[] =  "\nDone.$";
                //char buffer[1024];
               // cout<<"Doneeeeeeeeeeee"<<msg<<endl;
                send(hc->getSocket(), msg, sizeof(msg), 0);
                // recv(hc->getSocket(), buffer, sizeof(buffer), 0);
                break;
            }
            else{
                char buffer[1024];
                string msgStr = to_string(counter) + " " + type;
               // cout<<"sending this"<<msgStr<<endl;
                send(hc->getSocket(),msgStr.c_str(), msgStr.length(), 0 );
                recv(hc->getSocket(), buffer, sizeof(buffer), 0);
            }

        }
        recv(hc->getSocket(),buff,sizeof(buff),0);
       // cout<<"got out of the while"<<endl;
        if (counter == 0) {//that means i read 0 lines from the file
           // cout << "empty file or wrong path- closing the program" << endl;
            //return "invalid input!";
        }
    }

};
class choice5 : public command {
public :
    void execute(HandleClient* hc) override {
        //cout<<"in choice 5"<<endl;
        choice4 c4;
        std::thread ex5_thread(&choice4::execute,&c4,hc);
        ex5_thread.join();
    }
};
class choice8 : public command {
public :
    void execute(HandleClient* hc) override {
        //cout<<"in choice 8"<<endl;
    }
};

#endif //THREADS_PROJECT_HANDLECLIENT_H