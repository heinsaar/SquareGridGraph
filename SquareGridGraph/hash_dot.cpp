// HashDot.cpp

#include "hash_dot.h"
#include <fstream>
#include <sstream>

extern int X;
extern int Y;

void HashDot::random(int n, int m, int d)
{
    lines_.resize(m);

    for (int y = 0; y < m; ++y)
        for (int x = 0; x < n; ++x)
            lines_[y] += random_hash_dot(d);

    length_ = n;
    height_ = m;
}

void HashDot::readFrom(std::string fileName)
{
    std::ifstream hashdotFile(fileName);

    std::string line;
    while (std::getline(hashdotFile, line))
        lines_.push_back(line);

    X = length_ = line.length();
    Y = height_ = lines_.size();

    if ((lines_.end() - 1)->length() !=
        (lines_.end() - 2)->length())
        throw std::exception("Error: Final lines of hashdot file have different size.\n");
}

string HashDot::getLine(int n)        const { return lines_[n]; }
int    HashDot::height()               const { return height_; }
int    HashDot::length()               const { return length_; }
char   HashDot::random_hash_dot(int d) const { return (rand() % 200 < d) ? '#' : '.'; }
