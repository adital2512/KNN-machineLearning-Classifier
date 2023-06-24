
#ifndef THREADS_PROJECT_HANDLEFILE_H
#define THREADS_PROJECT_HANDLEFILE_H

#include <cstring>
#include "Kclosest.h"
#include  <fstream>
#include  <sstream>
class HandleFile
{
private:
    Kclosest* kList;
    string path;
public:
    bool checkSize(string str1, int okSize);
    string cutLine(string str1, vec& vct, bool& okVector);
    HandleFile(Kclosest* kList, string path);
    HandleFile();
    string readFile();
};

#endif //THREADS_PROJECT_HANDLEFILE_H
