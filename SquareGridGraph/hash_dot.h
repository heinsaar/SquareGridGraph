// HashDot.h

#ifndef _HASH_DOT_
#define _HASH_DOT_

#pragma warning (disable : 4786)

#include "help_funcs.h"

#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::string;
using std::vector;

class HashDot {
public:
    std::string get_line(int n) const;
    int         height()        const;
    int         length()        const;
    void        random(int n, int m, int d);
    void        write_to( std::string fileName);
    void        read_from(std::string fileName);

private:
    char random_hash_dot(int d) const;

    vector<string> lines_;
    int            height_ = 0;
    int            length_ = 0;
};


#endif // _HASH_DOT_