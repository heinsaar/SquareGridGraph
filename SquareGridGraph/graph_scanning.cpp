#include "graph.h"

// SCANNING THE DOWNTOWN
bool Town::is_free(const BlockSite& b_s)
{
    return b_s == nullptr;
}

bool Town::is_cross_point(const Block& b)
{
    return b.b_id_ == 0;
}

bool Town::is_on_building(const Scanner& s)
{
    return (**s).b_id_ != 0;
}

bool Town::on_different_buildings(const Scanner& a, const Scanner& b)
{
    return (**a).b_id_ != (**b).b_id_;
}

bool Town::is_line_segment(const Block& b)
{
    return (!b.up   && !b.down)
        || (!b.left && !b.right);
}
bool Town::found_building(const Scanner& sk, const Scanner& s)
{
    return is_on_building(sk) && on_different_buildings(sk, s);
}
bool Town::is_on_contour(const Scanner& s)
{
    return !is_free(*s) && (**s).b_id_ == 0 && count_free_directions(**s) > 0;
}

void Town::set_contour_id(const Scanner& s, const ID& id)
{
    Scanner e = s;
    Direction from = NONE;
    do {
        set_building_id(**e, id);
        move_clockwise(e, from);
    } while (e != s);
}

void Town::enumerate_buildings()
{
    Scanner s;
    ID id = 1;
    for (int y = 0; y < grid_.max_y(); ++y) {
        grid_.locate(s, 0, y);
        while (!s.is_rightmost()) {
            if (is_on_contour(s)) {                        
                set_contour_id(s, id);
                unvisited_.push_back(id++);
            }
            s.move_right();
        }
    }
}

Town::Buildingpos Town::get_building_location(const ID& n)
{
    Scanner s;
            
    for (int y = 0; y < grid_.max_y(); ++y) {
        grid_.locate(s, 0, y);
        while (!s.is_rightmost()) {
            if (!is_free(*s))
                if ((**s).b_id_ == n)
                    return s; // upper leftmost corner
            s.move_right();
        }
    }
}

Town::Buildingpos Town::seek_up(const Scanner& s, int n)
{
    Scanner sk = s;

    while (!sk.is_upmost() && n > 0) {
        sk.move_up(); --n;
        if (!is_free(*sk))
            if (found_building(sk, s))
                return sk;
    }
    return s; // did not find
}

Town::Buildingpos Town::seek_right(const Scanner& s, int n)
{
    Scanner sk = s;

    while (!sk.is_rightmost() && n > 0) {
        sk.move_right(); --n;                // TODO: Try to optimize with move_right(n)
        if (!is_free(*sk))                            
            if (found_building(sk, s))
                return sk;
    }
    return s; // did not find
}

Town::Buildingpos Town::seek_down(const Scanner& s, int n)
{
    Scanner sk = s;

    while (!sk.is_downmost() && n > 0) {
        sk.move_down(); --n;
        if (!is_free(*sk))
            if (found_building(sk, s))
                return sk;
    }
    return s;    // did not find
}

Town::Buildingpos Town::seek_left(const Scanner& s, int n)
{
    Scanner sk = s;

    while (!sk.is_leftmost() && n > 0) {
        sk.move_left(); --n;
        if (!is_free(*sk))
            if (found_building(sk, s))
                return sk;
    }
    return s;    // did not find
}

int Town::count_free_directions(const Block& b)
{
    short n = 0;

    if (!b.up)    ++n;
    if (!b.right) ++n;
    if (!b.down)  ++n;
    if (!b.left)  ++n;

    return n;
}

bool Town::is_bridge_start(const Block& b, Direction from)
{
// Is one of the panels actually a start of a bridge?
    switch (from) {
        case UP:    return b.right; // bridge on right?
        case RIGHT: return b.down;  // bridge on down?
        case DOWN:  return b.left;  // bridge on left?
        case LEFT:  return b.up;    // bridge on up?
    }
}

Town::BlockShape Town::block_shape(const Block& b)
{
    switch (count_free_directions(b)) {
        case 2:
            if (is_line_segment(b))
                return LINE_SEGMENT;
            return CORNER;
        case 1:
            return T_SHAPE;
        case 0:
            return CROSS;                
    }
}

// Determines the block type for a builder moving clockwise
Town::WallShape Town::scanner_view(const Block& b, Direction from)
{
    WallShape shape;
            
    switch (block_shape(b)) {

        case LINE_SEGMENT: // [[fallthrough]]
        case T_SHAPE: {
            shape = FLAT_WALL;
            if (is_bridge_start(b, from))
                shape = INTERNAL_CORNER;                    
            } break;
        case CROSS: {
            shape = INTERNAL_CORNER;
            if (is_cross_point(b))
                shape = FLAT_WALL;                    
            } break;
        case CORNER: {
            shape = EXTERNAL_CORNER;
            } break;
    }
    return shape;
}
