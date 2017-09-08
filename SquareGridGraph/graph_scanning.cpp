#include "graph.h"

// SCANNING THE DOWNTOWN
bool Town::is_free(const BlockSite& b_s)   { return b_s.used == false; }
bool Town::is_cross_point(const Block& b)  { return b.b_id_ == 0;      }
bool Town::is_on_building(const Walker& s) { return (*s).b_id_ != 0;   }

bool Town::on_different_buildings(const Walker& a, const Walker& b)
{
    return (*a).b_id_ != (*b).b_id_;
}

bool Town::is_line_segment(const Block& b)
{
    return (!b.up   && !b.down)
        || (!b.left && !b.right);
}
bool Town::found_building(const Walker& sk, const Walker& w)
{
    return is_on_building(sk) && on_different_buildings(sk, w);
}
bool Town::is_on_contour(const Walker& w)
{
    return !is_free(*w) && w->b_id_ == 0 && count_free_directions(*w) > 0;
}

void Town::set_contour_id(const Walker& w, const ID& id)
{
    Walker e = w;
    Direction from = Direction::NONE;
    do {
        set_building_id(*e, id);
        move_clockwise(e, from);
    } while (e != w);
}

void Town::enumerate_buildings()
{
    ID id = 1;
    for (int y = 0; y < grid_.max_y(); ++y) {
        Walker w = grid_.cell(0, y);
        while (w.x() != grid_.max_x()-1) {
            if (is_on_contour(w)) {                        
                set_contour_id(w, id);
                unvisited_.push_back(id++);
            }
            w.move_right();
        }
    }
}

Town::BuildingPos Town::get_building_location(const ID& n)
{
    for (int y = 0; y < grid_.max_y(); ++y) {
        Walker w = grid_.cell(0, y);
        while (w.x() != grid_.max_x()-1) {
            if (!is_free(*w))
                if (w->b_id_ == n)
                    return w; // upper leftmost corner
            w.move_right();
        }
    }
    return BuildingPos();
}

Town::BuildingPos Town::seek_up(const Walker& w, int n)
{
    // TODO: Make this code work for it to replace the while loop below.
    //Walker sk = grid_.at(w.x(), w.y() - n);
    //if (sk.is_valid() && !grid_.is_upmost(sk))
    //    if (!is_free(*sk) && found_building(sk, w))
    //        return sk;

    Walker sk = w;
    while (!grid_.is_upmost(sk) && n > 0) {
        sk.move_up(); --n;
        if (!is_free(*sk))
            if (found_building(sk, w))
                return sk;
    }
    return w; // did not find
}

Town::BuildingPos Town::seek_right(const Walker& w, int n)
{
    Walker sk = w;

    while (!grid_.is_rightmost(sk) && n > 0) {
        sk.move_right(); --n;                // TODO: Try to optimize with move_right(n)
        if (!is_free(*sk))                            
            if (found_building(sk, w))
                return sk;
    }
    return w; // did not find
}

Town::BuildingPos Town::seek_down(const Walker& w, int n)
{
    Walker sk = w;

    while (!grid_.is_downmost(sk) && n > 0) {
        sk.move_down(); --n;
        if (!is_free(*sk))
            if (found_building(sk, w))
                return sk;
    }
    return w; // did not find
}

Town::BuildingPos Town::seek_left(const Walker& w, int n)
{
    Walker sk = w;

    while (!grid_.is_leftmost(sk) && n > 0) {
        sk.move_left(); --n;
        if (!is_free(*sk))
            if (found_building(sk, w))
                return sk;
    }
    return w; // did not find
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
        case Direction::UP:    return b.right; // bridge on right?
        case Direction::RIGHT: return b.down;  // bridge on down?
        case Direction::DOWN:  return b.left;  // bridge on left?
        case Direction::LEFT:  return b.up;    // bridge on up?
        default: throw sgg::Error("Unexpected 'NONE' direction 'from'.");
    }
}

Town::BlockShape Town::block_shape(const Block& b)
{
    switch (count_free_directions(b)) {
        case 2: if (is_line_segment(b)) return BlockShape::LINE_SEGMENT;
                return BlockShape::CORNER;
        case 1: return BlockShape::T_SHAPE;
        case 0: return BlockShape::CROSS;
    }
    return BlockShape::NONE;
}

// Determines the block type for a builder moving clockwise
Town::WallShape Town::scanner_view(const Block& b, Direction from)
{
    WallShape shape = WallShape::NONE;
            
    switch (block_shape(b)) {

        case BlockShape::LINE_SEGMENT: [[fallthrough]];
        case BlockShape::T_SHAPE: {
            shape = WallShape::FLAT_WALL;
            if (is_bridge_start(b, from))
                shape = WallShape::INTERNAL_CORNER;
            } break;
        case BlockShape::CROSS: {
            shape = WallShape::INTERNAL_CORNER;
            if (is_cross_point(b))
                shape = WallShape::FLAT_WALL;
            } break;
        case BlockShape::CORNER: {
            shape = WallShape::EXTERNAL_CORNER;
            } break;
    }
    return shape;
}
