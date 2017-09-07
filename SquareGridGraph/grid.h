#ifndef GRID_H
#define GRID_H

#include <vector>
#include <cmath> // abs()

extern int X;
extern int Y;

template<class Ker>
    class Grid {
protected:
    struct Node;

    using Location = std::pair<int, int>;
    using Coord    = int;
    using Imp      = std::vector<Node>;
    using Impit    = typename Imp::iterator;

    struct Node {
        Node() = default;
        Node(const Ker& k) : kernel(k)
        { up = right = down = left = Imp::iterator(); }

        Impit up, right, down, left;
        Ker kernel;
    };

    friend struct Acc;

    struct Acc {
        using Impitref  = Impit&;
        using Kernelref = Ker&;

        static Impitref  to(Coord x, Coord y) { return ((Impitref) it->left);   }
        static Impitref  left(  Impit n)      { return ((Impitref) n->left);   }
        static Impitref  right( Impit n)      { return ((Impitref) n->right);  }
        static Impitref  up(    Impit n)      { return ((Impitref) n->up);     }
        static Impitref  down(  Impit n)      { return ((Impitref) n->down);   }
        static Kernelref kernel(Impit n)      { return ((Kernelref)n->kernel); }
    };

public:
    using Kptr            =       Ker*;
    using Ckptr           = const Ker*;
    using reference       =       Ker&;
    using const_reference = const Ker&;

           class       walker;
           class const_walker;
    friend class const_walker;

    class const_walker {
    public:
        const_walker() = default;
        const_walker(Impit p, Coord x, Coord y) { nodeit_ = p; x_ = x; y_ = y; }
        const_walker(const walker& w) : nodeit_(w.nodeit_), x_(w.x_), y_(w.y_){}
        const_reference operator*() const { return (Acc::kernel(nodeit_)); }
        Ckptr operator->()          const { return (&**this); }

    // HORIZONTAL / VERTICAL MOVE
        const_walker& move_left(int n = 1)
        {
            for (; 0 < n; --n)
            {
                nodeit_ = Acc::left(nodeit_);
                --x_;
            }
            return *this;;
        }
        const_walker& move_right(int n = 1)
        {
            for (; 0 < n; --n)
            {
                nodeit_ = Acc::right(nodeit_);
                ++x_;
            }
            return *this;;
        }
        const_walker& move_up(int n = 1)
        {
            for (; 0 < n; --n)
            {
                nodeit_ = Acc::up(nodeit_);
                --y_;
            }
            return *this;;
        }
        const_walker& move_down(int n = 1)
        {
            for (; 0 < n; --n)
            {
                nodeit_ = Acc::down(nodeit_);
                ++y_;
            }
            return *this;;
        }

    // LOCATION
        bool is_upmost()    const { return y_ == 0; }
        bool is_rightmost() const { return x_ == X; } // max_x(); } // TODO: make work with max_x() (remove global X and Y)
        bool is_downmost()  const { return y_ == Y; } // max_y(); } // TODO: make work with max_y() (remove global X and Y)
        bool is_leftmost()  const { return x_ == 0; }
        bool is_at_edge()   const { return (is_upmost() || is_leftmost() || is_rightmost() || is_downmost()); }
        Location location() const { return { x_, y_ }; }
        Coord x()           const { return x_; }
        Coord y()           const { return y_; }
        
    protected:
        enum { INVALID = -1 };

        Impit nodeit_;
        Coord x_ = INVALID;
        Coord y_ = INVALID;
    };

    friend class walker;
    class walker : public const_walker {
    public:
        walker() {}
        walker(Impit it, Coord x, Coord y) { nodeit_ = it; x_ = x; y_ = y;  }
        reference operator*() const        { return (Acc::kernel(nodeit_)); }
        Kptr operator->()     const        { return (&**this); }

    // HORIZONTAL / VERTICAL MOVE
        walker& move_left(int n = 1)
        {
            for (; 0 < n; --n)
            {
                nodeit_ = Acc::left(nodeit_);
                --x_;
            }
            return *this;;
        }
        walker& move_right(int n = 1)
        {
            for (; 0 < n; --n)
            {
                nodeit_ = Acc::right(nodeit_);
                ++x_;
            }
            return *this;;
        }
        walker& move_up(int n = 1)
        {
            for (; 0 < n; --n)
            {
                nodeit_ = Acc::up(nodeit_);
                --y_;
            }
            return *this;;
        }
        walker& move_down(int n = 1)
        {
            for (; 0 < n; --n)
            {
                nodeit_ = Acc::down(nodeit_);
                ++y_;
            }
            return *this;;
        }

        bool operator==(const walker& w) const { return (nodeit_ == w.nodeit_); }
        bool operator!=(const walker& w) const { return (!(*this == w)); }
    };

    Grid(int x, int y) : max_x_(x), max_y_(y), data_(x * y)
    {
        connect_horizontally();
        connect_vertically();
    }

// ACCESSORS
    int max_x() const { return max_x_; }
    int max_y() const { return max_y_; }

// LOCATION RELATED
    const_walker at(Coord x, Coord y) const { return const_walker(data_.begin() + y + (max_x_-1) * y, x, y); }
          walker at(Coord x, Coord y)       { return       walker(data_.begin() + y + (max_x_-1) * y, x, y); }

    int distance(const_walker& a, const_walker& b)
    {
        const int dx = a.x() - b.x();
        const int dy = a.y() - b.y();
        return (dx != 0) ? abs(dx) : abs(dy);
    }

    int distance(walker& a, walker& b)
    {
        const int dx = a.x() - b.x();
        const int dy = a.y() - b.y();
        return (dx != 0) ? abs(dx) : abs(dy);
    }

protected:

// CONNECTING THE NODES
    void connect_right(Impit curr)
    {
        Impit right = curr + 1;
        curr->right = right;
        right->left = curr;
    }

    void connect_down(Impit curr)
    {
        Impit down = curr + max_x_;
        curr->down = down;
        down->up = curr;
    }

    void connect_horizontally()
    {
        for (Impit y = data_.begin(); y < data_.end(); y += max_x_)
            for (Impit x = y; x < y + max_x_ - 1; ++x)
                connect_right(x);
    }

    void connect_vertically()
    {
        for (Impit y = data_.begin(); y < data_.end() - max_x_; y += max_x_)
            for (Impit x = y; x < y + max_x_; ++x)
                connect_down(x);
    }

private:
    const Coord max_x_;
    const Coord max_y_;
          Imp   data_;
};

#endif // GRID_H