#ifndef RANDOM_H
#define RANDOM_H

#include <random>

class Random
{
public:
    Random(){};
    double generate_double(double lower_boundary, double upper_boundary);
    int generate_integer(int lower_boundary, int upper_boundary);
};

#endif // RANDOM_H
