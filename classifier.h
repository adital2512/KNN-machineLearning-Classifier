
#ifndef THREADS_PROJECT_CLASSIFIER_H
#define THREADS_PROJECT_CLASSIFIER_H

#include "HandleFile.h"
class classifier {
    string handleFullInput(char* ,int size, string path);
    int split_string(string str, string seperator, string& strVector, string& strK);
public:
    string fillVector(string str1, vec& vct);
    string handleInput(string str1, string distanceFormat, int k, string path);
};

#endif //THREADS_PROJECT_CLASSIFIER_H
