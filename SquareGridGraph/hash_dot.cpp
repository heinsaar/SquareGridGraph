#include "hash_dot.h"
#include <fstream>
#include <sstream>

using namespace sgg;

extern int X;
extern int Y;

HashDot::HashDot(std::string fileName)
{
    if (!fileName.empty())
        read_from(fileName);
}

void HashDot::write_to(std::string name)
{
    std::ofstream file(name);
    if (!file)
        throw Error("Unable to open file " + quote(name) + ".");

    for (int i = 0; i < lines_.size(); i++)
    {
        file << lines_[i];
        if (i != lines_.size() - 1)
            file << std::endl;
    }
}

void HashDot::random(int n, int m, int d)
{
    lines_.resize(m);

    for (int y = 0; y < m; ++y)
    for (int x = 0; x < n; ++x)
        lines_[y] += random_hash_dot(d);

    length_ = n;
    height_ = m;
}

void HashDot::read_from(std::string fileName)
{
    lines_.clear();

    std::ifstream hashdotFile(fileName);
    if (!hashdotFile)
        throw Error("Error: Expected hashdot file " + quote(fileName) + " does not exist.\n");

    std::string line = "";
    while (std::getline(hashdotFile, line))
    {
        if (!lines_.empty() && lines_.back().length() != line.length())
            throw Error("Not all lines of hashdot file " + quote(fileName) + " have equal length.\n");

        lines_.push_back(line);
    }

    X = length_ = line.length();
    Y = height_ = lines_.size();
}

std::string HashDot::get_line(int n)        const { return lines_[n]; }
int         HashDot::height()               const { return height_; }
int         HashDot::length()               const { return length_; }
char        HashDot::random_hash_dot(int d) const { return (rand(0, 1000) < d) ? '#' : '.'; }
