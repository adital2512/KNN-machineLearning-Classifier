
#ifndef THREADS_PROJECT_KCLOSEST_H
#define THREADS_PROJECT_KCLOSEST_H

#include "ExtendedVector.h"
class Kclosest
{
private:
    vector<ExtendedVector> v; // vector of extended vectors
    int k; //the size of v(k closest vectors)
    double (*distanceFunctionPtr)(vec, vec) = NULL; //the distance function
    vec ucVec; //the vector we need to calculate distance from.(unclassified vector)
public:
    int getVecSize();
    bool hasMethod();
    Kclosest(int k, string distanceMethod, vec ucVec);
    Kclosest();
    vector<ExtendedVector> getV();
    void addVec(vec v, string);
    string getMost();
};
#endif //THREADS_PROJECT_KCLOSEST_H
