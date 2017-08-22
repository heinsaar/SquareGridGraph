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
    string get_line(int n) const;
    int  height() const;
    int  length() const;
    void input_downtown_area();
    void input_hash_dot();
    void random(int n, int m, int d);

    HashDot& operator=(const HashDot& hd);

private:
    char random_hash_dot(int d) const;

    vector<string> store_;
    int            height_;
    int            length_;
};


#endif /* _HASH_DOT_ */