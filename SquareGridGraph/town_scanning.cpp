// Dtown_Scanning.cpp

#include "Town.h"

// SCANNING THE DOWNTOWN
bool Town::_Is_free(const _BlockSite& b_s)
{
    return b_s == nullptr;
}

bool Town::_Is_free(const _PanelSite& p_s)
{
    return p_s == nullptr;
}

bool Town::_Is_cross_point(const _Block& b)
{
    return b.b_id_ == 0;
}

bool Town::_Is_on_building(const _Scanner& s)
{
    return (**s).b_id_ != 0;
}

bool Town::_On_different_buildings(const _Scanner& a, const _Scanner& b)
{
    return (**a).b_id_ != (**b).b_id_;
}

bool Town::_Is_line_segment(const _Block& b)
{
    return (_Is_free(b.up)   && _Is_free(b.down))
        || (_Is_free(b.left) && _Is_free(b.right));
}
bool Town::_Found_building(const _Scanner& sk, const _Scanner& s)
{
    return _Is_on_building(sk) && _On_different_buildings(sk, s);
}
bool Town::_Is_on_contour(const _Scanner& s)
{
    return !_Is_free(*s) && (**s).b_id_ == 0 && _Count_free_directions(**s) > 0;
}

void Town::_Set_contour_id(const _Scanner& s, const _ID& id)
{
    _Scanner e = s;
    _Direction from = NONE;
    do {
        _Set_building_id(**e, id);
        _Move_clockwise(e, from);
    } while (e != s);
}

void Town::_Enumerate_buildings()
{
    _Scanner s;
    _ID id = 1;
    for (int y = 0; y < grid_.max_y(); ++y) {
        grid_.locate(s, 0, y);
        while (!s.is_rightmost()) {
            if (_Is_on_contour(s)) {                        
                _Set_contour_id(s, id);
                unvisited_.push_back(id++);
            }
            s.move_right();
        }
    }
}

Town::_Buildingptr Town::_Get_building_location(const _ID& n)
{
    _Scanner s;
            
    for (int y = 0; y < grid_.max_y(); ++y) {
        grid_.locate(s, 0, y);
        while (!s.is_rightmost()) {
            if (!_Is_free(*s))
                if ((**s).b_id_ == n)
                    return s; // upper leftmost corner
            s.move_right();
        }
    }
}

Town::_Buildingptr Town::_Seek_up(const _Scanner& s, int n)
{
    _Scanner sk = s;

    while (!sk.is_upmost() && n > 0) {
        sk.move_up(); --n;
        if (!_Is_free(*sk))
            if (_Found_building(sk, s))
                return sk;
    }
    return s;    // did not find
}

Town::_Buildingptr Town::_Seek_right(const _Scanner& s, int n)
{
    _Scanner sk = s;

    while (!sk.is_rightmost() && n > 0) {
        sk.move_right(); --n;
        if (!_Is_free(*sk))
            if (_Found_building(sk, s))
                return sk;
    }
    return s; // did not find
}

Town::_Buildingptr Town::_Seek_down(const _Scanner& s, int n)
{
    _Scanner sk = s;

    while (!sk.is_downmost() && n > 0) {
        sk.move_down(); --n;
        if (!_Is_free(*sk))
            if (_Found_building(sk, s))
                return sk;
    }
    return s;    // did not find
}

Town::_Buildingptr Town::_Seek_left(const _Scanner& s, int n)
{
    _Scanner sk = s;

    while (!sk.is_leftmost() && n > 0) {
        sk.move_left(); --n;
        if (!_Is_free(*sk))
            if (_Found_building(sk, s))
                return sk;
    }
    return s;    // did not find
}

int Town::_Count_free_directions(const _Block& b)
{
    short n = 0;

    if (_Is_free(b.up))    ++n;
    if (_Is_free(b.right)) ++n;
    if (_Is_free(b.down))  ++n;
    if (_Is_free(b.left))  ++n;

    return n;
}

bool Town::_Is_bridge_start(const _Block& b, _Direction from)
{
// Is one of the panels actually a start of a bridge?
    switch (from) {
        case UP:    return !_Is_free(b.right); // bridge on right?
        case RIGHT:    return !_Is_free(b.down);  // bridge on down?
        case DOWN:    return !_Is_free(b.left);  // bridge on left?
        case LEFT:    return !_Is_free(b.up);    // bridge on up?
    }
}

Town::_BlockShape Town::_Block_shape(const _Block& b)
{
    switch(_Count_free_directions(b)) {
        case 2:
            if (_Is_line_segment(b))
                return LINE_SEGMENT;
            return CORNER;
        case 1:
            return T_SHAPE;
        case 0:
            return CROSS;                
    }
}

Town::_WallShape Town::_Scanner_view(const _Block& b, _Direction from)
{
// What is the block for a builder moving clockwise?
    _WallShape at;
            
    switch(_Block_shape(b)) {

        case LINE_SEGMENT: case T_SHAPE: {
            at = FLAT_WALL;
            if (_Is_bridge_start(b, from))
                at = INTERNAL_CORNER;                    
            } break;
        case CROSS: {
            at = INTERNAL_CORNER;
            if (_Is_cross_point(b))
                at = FLAT_WALL;                    
            } break;
        case CORNER: {
            at = EXTERNAL_CORNER;
            } break;
    }
    return at;
}
