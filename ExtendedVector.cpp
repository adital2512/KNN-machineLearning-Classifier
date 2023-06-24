#include "ExtendedVector.h"
using namespace std;
/*
constructor:
assign the rigth values and the distance from ucvec according to the distanceMethod
*/
ExtendedVector::ExtendedVector(vec v, string kind, double distance) {
    this->v = v;
    this->kind = kind;
    this->distance = distance;
}
ExtendedVector::ExtendedVector() {};

string ExtendedVector::getKind() {
    return this->kind;
}


double ExtendedVector::getDistance() {
    return this->distance;
}