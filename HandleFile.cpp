#include "HandleFile.h"

HandleFile::HandleFile(Kclosest* kList, string path) {
	this->kList = kList;
	this->path = path;
}


HandleFile::HandleFile() {};


string HandleFile::readFile() {
	int vecSize = (*kList).getVecSize();
	ifstream myFile;
	try {
		myFile.open(this->path);
	}
	catch (const std::exception& ex) {
		cout << "Wrong path to file! there is no a file in this path";
		return "invalid input!";
	}
	int count = 0;// check how may lines we read from file
	while (myFile.good()) {
		count++;
		string line;
		string vecNumbers;
		getline(myFile, line);
		vec v;
		//cout << checkSize(line, 4) << endl;
		bool okVector = checkSize(line, vecSize);
		string type = cutLine(line, v, okVector);
		//v and string are ready, can be send
		//cout << type << endl;

		if (okVector) {
			this->kList->addVec(v, type);
		}
		//cout << "new line" << endl;
	}
	if(count==0){//that means i read 0 lines from the file
		cout << "empty file or wrong path- closing the program" << endl;
		return "invalid input!";
    }
	return "1";
}
// check if the vector size is ok /
bool HandleFile::checkSize(string str1, int okSize) {
	string s;
	int count = 0;
	for (auto x : str1)
	{
		if (x == ',')
		{
			count++;
			s = "";
		}
		else {
			s = s + x;
		}
	}
	return count == okSize;

}


string HandleFile::cutLine(string str1, vec & vct, bool& okVector) {
	string s;
	double n1;
	for (auto x : str1)
	{
		if (x == ',')
		{
			try {
				//cout << s << endl;
				n1 = stod(s);
			}
			catch (const std::exception& ex) {
				okVector = false;
				break;
			}
			vct.push_back(n1);
			s = "";
		}
		else {
			s = s + x;
		}
	}
	return s;
}
