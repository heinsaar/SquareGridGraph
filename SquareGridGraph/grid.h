// Grid.h

#ifndef _CARTESIAN_LATTICE_
#define _CARTESIAN_LATTICE_

#include <cmath> // abs()
#include <vector>
#include <utility>

using std::pair;
using std::vector;

extern int X;
extern int Y;

template<class Ker>
class Grid {
protected:
    struct Node;

    using Imp      = vector<Node>;
    using Nodeiter = typename Imp::iterator;
    using Location = pair<int, int>;

    struct Node {
        Node(const Ker& k = Ker()) : kernel(k)
        { up = right = down = left = Imp::iterator(); }

        Nodeiter up, right, down, left;
        Ker kernel;
    };

    friend struct Acc;

    struct Acc {
        using Nodeitref = Nodeiter&;
        using Kernelref = Ker&;
        static Nodeitref left(  Nodeiter p) { return ((Nodeitref)p->left);   }
        static Nodeitref right( Nodeiter p) { return ((Nodeitref)p->right);  }
        static Nodeitref up(    Nodeiter p) { return ((Nodeitref)p->up);     }
        static Nodeitref down(  Nodeiter p) { return ((Nodeitref)p->down);   }
        static Kernelref kernel(Nodeiter p) { return ((Kernelref)p->kernel); }
    };

public:
    using Kptr            =       Ker*;
    using Ckptr           = const Ker*;
    using reference       =       Ker&;
    using const_reference = const Ker&;
    using Coord           = int;

           class       walker;
           class const_walker;
    friend class const_walker;

    class const_walker {
    public:
        const_walker() {}
        const_walker(Nodeiter p, Coord x, Coord y) { curr_ = p; x_ = x; y_ = y; }
        const_walker(const walker& w) : curr_(w.curr_), x_(w.x_), y_(w.y_){}
        const_reference operator*() const { return (Acc::kernel(curr_)); }
        Ckptr operator->()          const { return (&**this); }

    // HORIZONTAL / VERTICAL MOVE
        const_walker& move_left(int n = 1)
        {
            for (; 0 < n; --n)
            {
                curr_ = Acc::left(curr_);
                --x_;
            }
            return *this;;
        }
        const_walker& move_right(int n = 1)
        {
            for (; 0 < n; --n)
            {
                curr_ = Acc::right(curr_);
                ++x_;
            }
            return *this;;
        }
        const_walker& move_up(int n = 1)
        {
            for (; 0 < n; --n)
            {
                curr_ = Acc::up(curr_);
                --y_;
            }
            return *this;;
        }
        const_walker& move_down(int n = 1)
        {
            for (; 0 < n; --n)
            {
                curr_ = Acc::down(curr_);
                ++y_;
            }
            return *this;;
        }

    // LOCATION
        bool is_upmost()     const { return y_ == 0;  }
        bool is_rightmost()  const { return x_ == X; } // max_x(); } // Leo: generalize back to max_x()
        bool is_downmost()   const { return y_ == Y; } // max_y(); } // Leo: generalize back to max_y()
        bool is_leftmost()   const { return x_ == 0;  }
        bool is_at_edge()    const { return (is_upmost() || is_leftmost() || is_rightmost() || is_downmost()); }
        Location location()  const { return { x_, y_ }; }
        Coord x()            const { return x_; }
        Coord y()            const { return y_; }
        
    // OPERATORS
        bool operator==(const const_walker& w) const { return (curr_ == w.curr_); }
        bool operator!=(const const_walker& w) const { return (!(*this == w)); }
    protected:
        Nodeiter curr_;
        Coord       x_;
        Coord       y_;
    };

        // CLASS walker
    friend class walker;
    class walker : public const_walker {
    public:
        walker() {}
        walker(Nodeiter p, Coord x, Coord y) { curr_ = p; x_ = x; y_ = y; }
        reference operator*() const          { return (Acc::kernel(curr_)); }
        Kptr operator->()     const          { return (&**this); }

    // HORIZONTAL / VERTICAL MOVE
        walker& move_left(int n = 1)
            {for (; 0 < n; --n)
                {curr_ = Acc::left(curr_);
                    --x_; }                
            return *this;; }
        walker& move_right(int n = 1)
            {for (; 0 < n; --n)
                {curr_ = Acc::right(curr_);
                    ++x_; }
            return *this;; }
        walker& move_up(int n = 1)
            {for (; 0 < n; --n)
                {curr_ = Acc::up(curr_);
                    --y_; }
            return *this;; }
        walker& move_down(int n = 1)
            {for (; 0 < n; --n)
                {curr_ = Acc::down(curr_);
                    ++y_; }
            return *this;; }

    // DIAGONAL MOVE
        walker& move_upright(int n = 1)   { return this->move_up(n).move_right(n); }
        walker& move_downright(int n = 1) { return this->move_down(n).move_right(n); }
        walker& move_downleft(int n = 1)  { return this->move_down(n).move_left(n); }
        walker& move_upleft(int n = 1)    { return this->move_up(n).move_left(n); }

        bool operator==(const walker& w) const { return (curr_ == w.curr_); }
        bool operator!=(const walker& w) const { return (!(*this == w)); }
    };

    Grid() : grid_() {}
    Grid(int x, int y) : max_x_(x), max_y_(y), grid_(x * y)
    {
        connect_horizontally();
        connect_vertically();
    }

// ACCESSORS
    int max_x() const { return max_x_; }
    int max_y() const { return max_y_; }

// LOCATION RELATED
    const_walker zero() const { return const_walker(grid_.begin(), 0, 0); }
          walker zero()       { return       walker(grid_.begin(), 0, 0); }

    int distance(const_walker& a, const_walker& b)
    {
        int dx = a.x() - b.x();
        int dy = a.y() - b.y();
        return (dx != 0) ? abs(dx) : abs(dy);
    }

    int distance(walker& a, walker& b)
    {
        int dx = a.x() - b.x();
        int dy = a.y() - b.y();
        return (dx != 0) ? abs(dx) : abs(dy);
    }

    void locate(const_walker& w, Coord x, Coord y)
    {
        w = zero();
        w.move_right(x).move_down(y);
    }

    void locate(walker& w, Coord x, Coord y)
    {
        w = zero();
        w.move_right(x).move_down(y);
    }

protected:

// CONNECTING THE NODES
    void connect_right(Nodeiter curr)
    {
        Nodeiter right = curr + 1;
        curr->right = right;
        right->left = curr;
    }

    void connect_down(Nodeiter curr)
    {
        Nodeiter down = curr + max_x_;
        curr->down = down;
        down->up = curr;
    }

    void connect_horizontally()
    {
        for (Nodeiter y = grid_.begin(); y < grid_.end(); y += max_x_)
            for (Nodeiter x = y; x < y + max_x_ - 1; ++x)
                connect_right(x);
    }

    void connect_vertically()
    {
        for (Nodeiter y = grid_.begin(); y < grid_.end() - max_x_; y += max_x_)
            for (Nodeiter x = y; x < y + max_x_; ++x)
                connect_down(x);
    }

private:
    Coord max_x_;
    Coord max_y_;
    Imp   grid_;
};

#endif // _CARTESIAN_LATTICE_