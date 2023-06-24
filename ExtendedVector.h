
#ifndef THREADS_PROJECT_EXTENDEDVECTOR_H
#define THREADS_PROJECT_EXTENDEDVECTOR_H


#include "calcs.h"
using namespace std;
class ExtendedVector {
private:
    vec v; //the vector parameters.
    string kind; //the kind of flower.
    double distance; //the distance between v to ucVec.
public:
    /*
    constructor:
    assign the rigth values and the distance from ucvec according to the distanceMethod
    */
    ExtendedVector(vec v, string kind, double distance);
    ExtendedVector();
    string getKind();
    double getDistance();
};

#endif //THREADS_PROJECT_CLASSIFIER_H