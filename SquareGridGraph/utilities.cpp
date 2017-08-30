#include "utilities.h"

int sgg::rand(int min, int max, bool seed)
{
    std::random_device device; // to seed
    static thread_local std::mt19937 gensame;
    static thread_local std::mt19937 genrand(device());
    std::uniform_int_distribution<int> distribution(min, max);
    return seed ? distribution(genrand) : distribution(gensame);
}

bool sgg::files_identical(std::string fileNameA, std::string fileNameB)
{
    std::ifstream A(fileNameA);
    if (!A)
        display_line("Error: Expected file " + quote(fileNameA) + " does not exist.");

    std::ifstream B(fileNameB);
    if (!B)
        display_line("Error: Expected file " + quote(fileNameB) + " does not exist.");

    std::stringstream a;
    std::stringstream b;

    a << A.rdbuf();
    b << B.rdbuf();

    return A && B && (a.str() == b.str());
}