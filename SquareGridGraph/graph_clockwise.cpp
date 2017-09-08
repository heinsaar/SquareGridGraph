#include "graph.h"

// MOVING CLOCKWISE
Town::Direction Town::first_move_direction(const Block& b)
{
    return first_from_external_corner(b);
}

Town::Direction Town::first_from_external_corner(const Block& b)
{
    // always one type of EXTERNAL_CORNER
    // (for the main algorithm using the
    // current get_building_location method)

    if (!b.left && !b.up && b.right)
        return Direction::RIGHT;

    return Direction::NONE;

    // other types of EXTERNAL_CORNER
    // (present here for extensibility)
/*        if (is_free(b.up)  &&
            is_free(b.right) &&
            !is_free(b.down))
            return DOWN;
        if (is_free(b.right) &&
            is_free(b.down)  &&
            !is_free(b.left))
            return LEFT;
        if (is_free(b.down) &&
            is_free(b.left) &&
            !is_free(b.up))
            return UP;            */
}

Town::Direction Town::external_corner_direction(const Direction from)
{
    switch (from) {                
        case Direction::UP:    return Direction::LEFT;
        case Direction::RIGHT: return Direction::UP;
        case Direction::DOWN:  return Direction::RIGHT;
        case Direction::LEFT:  return Direction::DOWN;
        default: return Direction::NONE;
    }
}

Town::Direction Town::internal_corner_direction(const Direction from)
{
    switch (from) {                
        case Direction::UP:    return Direction::RIGHT;
        case Direction::RIGHT: return Direction::DOWN;
        case Direction::DOWN:  return Direction::LEFT;
        case Direction::LEFT:  return Direction::UP;
        default: return Direction::NONE;
    }
}

Town::Direction Town::flat_wall_direction(const Direction from)
{
    switch (from) {
        case Direction::UP:    return Direction::DOWN;
        case Direction::RIGHT: return Direction::LEFT;
        case Direction::DOWN:  return Direction::UP;
        case Direction::LEFT:  return Direction::RIGHT;
        default: return Direction::NONE;
    }
}

Town::Direction Town::orient(const Block& b, Direction& from)
{
    if (from == Direction::NONE)
        return first_move_direction(b);

    switch (scanner_view(b, from)) {
        case WallShape::EXTERNAL_CORNER: return external_corner_direction(from);
        case WallShape::INTERNAL_CORNER: return internal_corner_direction(from);
        case WallShape::FLAT_WALL:       return flat_wall_direction(from);
        default: return Direction::NONE;
    }
}

void Town::move(Walker& w, Direction& to)
{
    Direction& from = to;
    switch (to) {
        case Direction::UP:    { w.move_up();    from = Direction::DOWN;  } break;
        case Direction::RIGHT: { w.move_right(); from = Direction::LEFT;  } break;
        case Direction::DOWN:  { w.move_down();  from = Direction::UP;    } break;
        case Direction::LEFT:  { w.move_left();  from = Direction::RIGHT; } break;
    }
}

void Town::move_clockwise(Walker& w, Direction& from)
{
    Direction& to = from;
               to = orient(**w, from);
     move(w,   to);
}