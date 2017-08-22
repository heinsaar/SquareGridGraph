// HashDot.cpp

#include "hash_dot.h"

void HashDot::input_downtown_area()
{
    int m;

    display("\nDowntown area: ");
    cin >> m;

    height_ = m;
    store_.resize(m);
}

void HashDot::input_hash_dot()
{
    display("Hash dot layout:\n\n");
    for (int y = 0; y < height_; ++y)
        cin >> store_[y];

    length_ = get_line(0).size();
}

void HashDot::random(int n, int m, int d)
{
    store_.resize(m);

    for (int y = 0; y < m; ++y)
        for (int x = 0; x < n; ++x)
            store_[y] += random_hash_dot(d);

    length_ = n;
    height_ = m;
}

HashDot& HashDot::operator=(const HashDot& hd)
{
    if (this != &hd)
    {
        store_  = hd.store_;
        height_ = hd.height_;
        length_ = hd.length_;
    }
    return *this;
}

string HashDot::get_line(int n)      const { return store_[n]; }
int HashDot::height()                const { return height_; }
int HashDot::length()                const { return length_; }
char HashDot::random_hash_dot(int d) const { return (rand() % 200 < d) ? '#' : '.'; }
