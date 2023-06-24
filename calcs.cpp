#include "calcs.h"
#define P 4
using namespace std;
typedef std::vector<double> vec;
double EuclideanDistance(vec a, vec b) {
    int size = (int)a.size();
    double sum = 0;
    for (int i = 0; i < size; i++) {
        sum += pow(a[i] - b[i], 2);
    }
    return sqrt(sum);
}
double ManhattanDistance(vec a, vec b) {
    int size = (int)a.size();
    double sum = 0;
    for (int i = 0; i < size; i++) {
        sum += abs(a[i] - b[i]);
    }

    return sum;
}
double ChebyshevDistance(vec a, vec b) {
    int size = (int)a.size();
    double maxi = 0;
    for (int i = 0; i < size; i++) {
        maxi = max(maxi, abs(a[i] - b[i]));
    }
    return maxi;
}
double CanberraDistance(vec a, vec b) {
    int size = (int)a.size();
    double sum = 0;
    for (int i = 0; i < size; i++) {
        if (a[i] != 0 || b[i] != 0) {
            sum += (abs(a[i] - b[i])) / (abs(a[i]) + abs(b[i]));
        }
        else {
            cout << "cannot calculate Canberra distance between these vectors - cannot devide by zero\n";
            return -1;
        }
    }
    return sum;
}
double MinkowskiDistance(vec a, vec b) {
    if (P == 0) {
        cout << "cannot calculate Minkowski distance with the current value of P - cannot devide by zero\n";
    }
    int size = (int)a.size();
    double sum = 0;
    for (int i = 0; i < size; i++) {
        sum += pow(abs(a[i] - b[i]), P);
    }
    return pow(sum, 1.0 / P);
}