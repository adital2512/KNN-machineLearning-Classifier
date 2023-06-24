#include "Kclosest.h"
Kclosest::Kclosest() {
    this->distanceFunctionPtr = nullptr;
    this->k = 0;
};
Kclosest::Kclosest(int k, string distanceMethod, vec ucVec) {
    this->k = k;
    this->ucVec = ucVec;
    if (distanceMethod == "AUC") {
        this->distanceFunctionPtr = EuclideanDistance;
    }
    else if (distanceMethod == "MAN") {
        this->distanceFunctionPtr = ManhattanDistance;
    }
    else if (distanceMethod == "CHB") {
        this->distanceFunctionPtr = ChebyshevDistance;
    }
    else if (distanceMethod == "CAN") {
        this->distanceFunctionPtr = CanberraDistance;
    }
    else if (distanceMethod == "MIN") {
        this->distanceFunctionPtr = MinkowskiDistance;
    }
    else {
        this->distanceFunctionPtr = nullptr;
    }
}
bool Kclosest::hasMethod() {
    return this->distanceFunctionPtr != nullptr;
}
void Kclosest::addVec(vec v, string kind) {
    double distance = this->distanceFunctionPtr(v, this->ucVec);
    if (distance != -1) {
        if (int(this->v.size()) < this->k) {
            this->v.push_back(ExtendedVector(v, kind, distance));
        }
        else {
            double maxDistance = this->v[0].getDistance();
            int maxDistanceIndex = 0;
            for (int i = 0; i < k; i++) {
                if (this->v[i].getDistance() > maxDistance) {
                    maxDistanceIndex = i;
                    maxDistance = this->v[i].getDistance();
                }
            }
            if (maxDistance > distance) {
                this->v[maxDistanceIndex] = ExtendedVector(v, kind, distance);
            }
        }
    }
}

int Kclosest::getVecSize() {
    return int(this->ucVec.size());
}  


string Kclosest::getMost() {
    vector<string> kinds;
    vector<int> count; //the number of kinds[i] = count[i]
    int alreadyExist = 0;
    for (int i = 0; i < int(this->v.size()); i++) { //iterate through v (k closest) and add the kind to the count and kinds vectors
        for (int j = 0; j < int(kinds.size()); j++) { //iteraete through kinds, if already exist add the count, else add the 
            //kind to the kinds vector and the count of it is 1.
            if (this->v[i].getKind() == kinds[j]) {
                count[j]++;
                alreadyExist = 1;
            }
        }
        if (!alreadyExist) {
            kinds.push_back(this->v[i].getKind()); //add new kind counter with count of 1
            count.push_back(1);
        }
        alreadyExist = 0;
    }


    int max = 0;
    for (int i = 0; i < int(count.size()); i++) {
        if (count[i] > count[max]) {
            max = i;
        }
    }
    //now the max is the index of the most common kind of flower in the kinds array
    if (kinds.size() == 0) {
        return "invalid input!";
    }
    return kinds[max];
}

