#include "random.h"

double Random::generate_double(double lower_boundary, double upper_boundary)
{
    // Create a random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // Define the range of the distribution
    std::uniform_real_distribution<double> dist(lower_boundary, upper_boundary);

    return dist(gen);
}

int Random::generate_integer(int lower_boundary, int upper_boundary)
{
    return std::round(generate_double(lower_boundary, upper_boundary));
}
