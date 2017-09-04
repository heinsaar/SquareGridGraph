#ifndef HASH_DOT_H
#define HASH_DOT_H

#include "rich_containers.h"
#include "utilities.h"

#include <iostream>
#include <string>
#include <vector>

class HashDot {
public:
    HashDot(std::string fileName = "");
    std::string get_line(int n) const;
    int         height()        const;
    int         length()        const;
    void        random(int n, int m, int d);
    void        write_to( std::string fileName);
    void        read_from(std::string fileName);

private:
    char random_hash_dot(int d) const;

    Vector<std::string> lines_;
    int                 height_ = 0;
    int                 length_ = 0;
};

#endif // HASH_DOT_H