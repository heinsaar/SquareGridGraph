#ifndef GRID_H
#define GRID_H

#include <vector>
#include <cmath> // abs()

template<class Ker>
    class Grid {
public:
    Grid(int x, int y) : max_x_(x), max_y_(y), data_(x * y)
    {
        connect_horizontally();
        connect_vertically();
    }

private:
    struct Node;

    using Location = std::pair<int, int>;
    using Coord    = int;
    using Vec      = std::vector<Node>;
    using Vecit    = typename Vec::iterator;

    struct Node {
        Node() = default;
        Node(const Ker& k) : kernel(k)
        { up = right = down = left = Vec::iterator(); }

        Vecit up, right, down, left;
        Ker kernel;
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
        const_walker(Vecit p, Coord x = INVALID, Coord y = INVALID) : nodeit_(p), x_(x), y_(y) {}
        const_walker(const walker& w) : nodeit_(w.nodeit_), x_(w.x_), y_(w.y_) {}
        const_reference operator*()  const { return nodeit_->kernel; }
        Ckptr           operator->() const { return &**this; }

    // HORIZONTAL / VERTICAL MOVE
        const_walker& move_left(int n = 1)
        {
            for (; 0 < n; --n)
            {
                nodeit_ = nodeit_->left;
                --x_;
            }
            return *this;
        }
        const_walker& move_right(int n = 1)
        {
            for (; 0 < n; --n)
            {
                nodeit_ = nodeit_->right;
                ++x_;
            }
            return *this;
        }
        const_walker& move_up(int n = 1)
        {
            for (; 0 < n; --n)
            {
                nodeit_ = nodeit_->up;
                --y_;
            }
            return *this;;
        }
        const_walker& move_down(int n = 1)
        {
            for (; 0 < n; --n)
            {
                nodeit_ = nodeit_->down;
                ++y_;
            }
            return *this;
        }

        bool is_valid_x()   const { return x_ != INVALID; }
        bool is_valid_y()   const { return y_ != INVALID; }
        bool is_valid()     const { return is_valid_x() && is_valid_y(); }
        Location location() const { return { x_, y_ }; }
        Coord x()           const { return x_; }
        Coord y()           const { return y_; }
        
    protected:
        enum { INVALID = -1 };

        Vecit nodeit_;  // TODO: Probably unnecessary, (x_, y_) pair already has all the info.
        Coord x_ = INVALID;
        Coord y_ = INVALID;
    };

    friend class walker;
    class walker : public const_walker {
    public:
        walker() = default;
        walker(Vecit it, Coord x = INVALID, Coord y = INVALID) : const_walker(it, x, y) {}
        walker(const walker& w) : const_walker(w) {}
        reference operator*()  const { return nodeit_->kernel; }
        Kptr      operator->() const { return &**this; }

    // HORIZONTAL / VERTICAL MOVE
        walker& move_left(int n = 1)
        {
            for (; 0 < n; --n)
            {
                nodeit_ = nodeit_->left;
                --x_;
            }
            return *this;;
        }
        walker& move_right(int n = 1)
        {
            for (; 0 < n; --n)
            {
                nodeit_ = nodeit_->right;
                ++x_;
            }
            return *this;;
        }
        walker& move_up(int n = 1)
        {
            for (; 0 < n; --n)
            {
                nodeit_ = nodeit_->up;
                --y_;
            }
            return *this;;
        }
        walker& move_down(int n = 1)
        {
            for (; 0 < n; --n)
            {
                nodeit_ = nodeit_->down;
                ++y_;
            }
            return *this;;
        }

        bool operator==(const walker& w) const { return nodeit_ == w.nodeit_; } // TODO: location() == w.location(); works, introduce later.
        bool operator!=(const walker& w) const { return !(*this == w); }
    };

    int max_x() const { return max_x_; }
    int max_y() const { return max_y_; }

// LOCATION RELATED
    bool is_rightmost(const const_walker& w) { return w.x() == max_x_- 1; }
    bool is_leftmost( const const_walker& w) { return w.x() == 0;         }
    bool is_upmost(   const const_walker& w) { return w.y() == 0;         }
    bool is_downmost( const const_walker& w) { return w.y() == max_y_- 1; }

    walker at(Coord x, Coord y)
    {
        // TODO: Or  max_*_- 1 ? Investigate.
        if (x < 0 || max_x_- 1 < x) return walker(data_.end()); // return invalid
        if (y < 0 || max_y_- 1 < y) return walker(data_.end()); // return invalid

        return cell(x, y);
    }

    const_walker cell(Coord x, Coord y) const { return const_walker(data_.begin() + y + (max_x_- 1) * y, x, y); }
          walker cell(Coord x, Coord y)       { return       walker(data_.begin() + y + (max_x_- 1) * y, x, y); }

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

private:

// CONNECTING THE NODES
    void connect_right(Vecit curr)
    {
        Vecit right = curr + 1;
        curr->right = right;
        right->left = curr;
    }

    void connect_down(Vecit curr)
    {
        Vecit down = curr + max_x_;
        curr->down = down;
        down->up = curr;
    }

    void connect_horizontally()
    {
        for (Vecit y = data_.begin(); y < data_.end(); y += max_x_)
            for (Vecit x = y; x < y + max_x_ - 1; ++x)
                connect_right(x);
    }

    void connect_vertically()
    {
        for (Vecit y = data_.begin(); y < data_.end() - max_x_; y += max_x_)
            for (Vecit x = y; x < y + max_x_; ++x)
                connect_down(x);
    }

private:
    const Coord max_x_;
    const Coord max_y_;
          Vec   data_;
};

#endif // GRID_H