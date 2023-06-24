#include "classifier.h"
string fillVector(string str1, vec& vct);
int classifier::split_string(string str, string seperator, string& strVector, string& strK) {
    std::string first_half;
    std::string second_half;

    size_t pos = str.find(seperator);
    if (pos != std::string::npos)
    {
        strVector = str.substr(0, pos);
        strK = str.substr(pos+5);
        return 1;
    }
    return 0;
}
string classifier::handleFullInput(char* buffer,int size, string path) {
    string s(buffer, size);
    string vectorString, kString;
    string seperators[] = {" MAN ", " AUC ", " CHB ", " CAN ", " MIN "};
    int i;
    for(i = 0;i< 5;i++) {
        if(1 == split_string(s, seperators[i], vectorString, kString)){
            break;
        }
    }
    int k;
    try {
        k = stoi(kString);
        if (k != (int)(stod(kString)) || k < 1) {
            cout << "k value must be positive integer! closing program now" << endl;
            return "invalid input!";
        }

    }
    catch (const std::exception& ex) {
        cout << "k value must be positive integer! closing program now" << endl;
        return "invalid input!";
    }
    string distanceFormat = (seperators[i]).substr(1, (seperators[i]).size() - 2);
    // cout <<"vec string:" << vectorString<<endl;
    // cout <<"distanceFormat" << distanceFormat<<endl;
    // cout <<"k" << k<<endl;

    // cout <<"path" << path<<endl;
    return handleInput(vectorString, distanceFormat, k, path);
}

string classifier::handleInput(string str1, string distanceFormat, int k, string path)
{
    vec v;
    string s = fillVector(str1, v);
    if (s != "1")
    {
        return s;
    }
    Kclosest* list = new Kclosest(k, distanceFormat, v);
    if (!list->hasMethod()) {
        cout << "name of distance method is wrong! closing the program now" << endl;
        return "invalid input!";
    }
    HandleFile hf = HandleFile(list, path);
    s = hf.readFile();
    if (s != "1")
    {
        return s;
    }
    string most = (*list).getMost();
    delete list;
    return most;
}
/* in this function i will fill the vector by the input that i got from the user.
* i will split the string by ' '. i will use double vectors
*/
string classifier::fillVector(string str1, vec& vct) {
    string s;
    double n1;
    for (auto x : str1)
    {
        if (x == ' ')
        {
            try {
                n1 = stod(s);
            }
            catch (const std::exception& ex) {
                cout << "Wrong input! you have to insert only numbers in the right form";
                return "invalid input!";
            }
            vct.push_back(n1);
            s = "";
        }
        else {
            s = s + x;
        }
    }
    try {
        n1 = stod(s);
    }
    catch (const std::exception& ex) {
        cout << "Wrong input! you have to insert only numbers in the right form";
        return "invalid input!";
    }
    vct.push_back(n1);
    return "1";
}