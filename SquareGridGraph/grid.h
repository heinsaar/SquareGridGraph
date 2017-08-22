// Grid.h

#ifndef _CARTESIAN_LATTICE_
#define _CARTESIAN_LATTICE_

#include <cmath> // fabs()
#include <vector>
#include <utility>

using std::pair;
using std::vector;

extern int X;
extern int Y;

template<class _Ker>
class Grid {
protected:
	struct _Node;

	using _Imp      = vector<_Node>;
	using _Nodeiter = typename _Imp::iterator;
	using _Location = pair<int,int>;

	// STRUCT _Node
	struct _Node {
		_Node(const _Ker& k = _Ker()) : kernel(k)
		{ up = right = down = left = _Imp::iterator(); }

		_Nodeiter up, right, down, left;
		_Ker kernel;
	};

	friend struct _Acc;

	struct _Acc {
		typedef _Nodeiter& _Nodeitref;
		typedef	_Ker& _Kernelref;
		static _Nodeitref left(  _Nodeiter p) { return ((_Nodeitref)p->left);   }
		static _Nodeitref right( _Nodeiter p) { return ((_Nodeitref)p->right);  }
		static _Nodeitref up(    _Nodeiter p) { return ((_Nodeitref)p->up);     }
		static _Nodeitref down(  _Nodeiter p) { return ((_Nodeitref)p->down);   }
		static _Kernelref kernel(_Nodeiter p) { return ((_Kernelref)p->kernel); }
	};

public:
	using _Kptr           =       _Ker*;
	using _Ckptr          = const _Ker*;
	using reference       =       _Ker&;
	using const_reference = const _Ker&;
	using _Coord          = int;

	class walker;
	class const_walker;
	friend class const_walker;

	class const_walker {
	public:
		const_walker() {}
		const_walker(_Nodeiter p, _Coord x, _Coord y) { curr_ = p; x_ = x; y_ = y; }
		const_walker(const walker& w) : curr_(w.curr_), x_(w.x_), y_(w.y_){}
		const_reference operator*() const { return (_Acc::kernel(curr_)); }
		_Ckptr operator->()         const { return (&**this); }

	// HORIZONTAL / VERTICAL MOVE
		const_walker& move_left(int n = 1)
        {
            for (; 0 < n; --n)
            {
                curr_ = _Acc::left(curr_);
                --x_;
            }
            return *this;;
        }
		const_walker& move_right(int n = 1)
        {
            for (; 0 < n; --n)
            {
                curr_ = _Acc::right(curr_);
                ++x_;
            }
            return *this;;
        }
        const_walker& move_up(int n = 1)
        {
            for (; 0 < n; --n)
            {
                curr_ = _Acc::up(curr_);
                --y_;
            }
            return *this;;
        }
        const_walker& move_down(int n = 1)
        {
            for (; 0 < n; --n)
            {
                curr_ = _Acc::down(curr_);
                ++y_;
            }
            return *this;;
        }

	// DIAGONAL MOVE
		const_walker& move_upright(int n = 1)   { return *this;.move_up(n).move_right(n);   }
		const_walker& move_downright(int n = 1) { return *this;.move_down(n).move_right(n); }
		const_walker& move_downleft(int n = 1)  { return *this;.move_down(n).move_left(n);  }
		const_walker& move_upleft(int n = 1)    { return *this;.move_up(n).move_left(n);    }

	// LOCATION
		bool is_upmost()     const { return y_ == 0;  }
        bool is_rightmost()  const { return x_ == X; } // max_x(); } // Leo: generalize back to max_x()
        bool is_downmost()   const { return y_ == Y; } // max_y(); } // Leo: generalize back to max_y()
		bool is_leftmost()   const { return x_ == 0;  }
		bool is_at_edge()    const { return	(is_upmost() || is_leftmost() || is_rightmost() || is_downmost()); }
        _Location location() const { return { x_, y_ }; }
		_Coord x()           const { return x_; }
		_Coord y()           const { return y_; }
		
	// OPERATORS
		bool operator==(const const_walker& w) const { return (curr_ == w.curr_); }
		bool operator!=(const const_walker& w) const { return (!(*this == w)); }
	protected:
		_Nodeiter curr_;
        _Coord       x_;
        _Coord       y_;
	};

		// CLASS walker
	friend class walker;
	class walker : public const_walker {
	public:
		walker() {}
		walker(_Nodeiter p, _Coord x, _Coord y) { curr_ = p; x_ = x; y_ = y; }
		reference operator*() const  			{ return (_Acc::kernel(curr_)); }
		_Kptr operator->()    const       		{ return (&**this); }

	// HORIZONTAL / VERTICAL MOVE
		walker& move_left(int n = 1)
			{for (; 0 < n; --n)
				{curr_ = _Acc::left(curr_);
					--x_; }				
			return *this;; }
		walker& move_right(int n = 1)
			{for (; 0 < n; --n)
				{curr_ = _Acc::right(curr_);
					++x_; }
			return *this;; }
		walker& move_up(int n = 1)
			{for (; 0 < n; --n)
				{curr_ = _Acc::up(curr_);
					--y_; }
			return *this;; }
		walker& move_down(int n = 1)
			{for (; 0 < n; --n)
				{curr_ = _Acc::down(curr_);
					++y_; }
			return *this;; }

	// DIAGONAL MOVE
		walker& move_upright(int n = 1)	  { return this->move_up(n).move_right(n); }
		walker& move_downright(int n = 1) { return this->move_down(n).move_right(n); }
		walker& move_downleft(int n = 1)  { return this->move_down(n).move_left(n); }
		walker& move_upleft(int n = 1)	  { return this->move_up(n).move_left(n); }

		bool operator==(const walker& w) const { return (curr_ == w.curr_); }
		bool operator!=(const walker& w) const { return (!(*this == w)); }
	};

	Grid() : grid_() {}
	Grid(int x, int y) : max_x_(x), max_y_(y), grid_(x * y)
    {
        _Connect_horizontally();
        _Connect_vertically();
    }

// ACCESSORS
	int max_x() const { return max_x_; }
	int max_y() const { return max_y_; }

// LOCATION RELATED
	const_walker zero() const { return const_walker(grid_.begin(), 0, 0); }
	      walker zero()		  { return       walker(grid_.begin(), 0, 0); }

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

	const_walker locate(const_walker& w, _Coord x, _Coord y)
    {
        w = zero();
        w.move_right(x).move_down(y);
        return w;
    }

	walker locate(walker& w, _Coord x, _Coord y)
    {
        w = zero();
        w.move_right(x).move_down(y);
        return w;
    }

protected:

// CONNECTING THE NODES
    void _Connect_right(_Nodeiter curr)
    {
        _Nodeiter right = curr + 1;
        curr->right = right;
        right->left = curr;
    }

    void _Connect_down(_Nodeiter curr)
    {
        _Nodeiter down = curr + max_x_;
        curr->down = down;
        down->up = curr;
    }

    void _Connect_horizontally()
    {
        for (_Nodeiter y = grid_.begin(); y < grid_.end(); y += max_x_)
            for (_Nodeiter x = y; x < y + max_x_ - 1; ++x)
                _Connect_right(x);
    }

	void _Connect_vertically()
    {
        for (_Nodeiter y = grid_.begin(); y < grid_.end() - max_x_; y += max_x_)
            for (_Nodeiter x = y; x < y + max_x_; ++x)
                _Connect_down(x);
    }

private:
	_Coord max_x_;
	_Coord max_y_;
	_Imp   grid_;
};

#endif /* _CARTESIAN_LATTICE_ */