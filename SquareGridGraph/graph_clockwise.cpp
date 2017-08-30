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

        if (is_free(b.left) &&
            is_free(b.up)   &&
            !is_free(b.right))
            return RIGHT;

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

Town::Direction Town::external_corner_direction(const Direction& from)
{
    switch (from) {                
        case UP:    return LEFT;
        case RIGHT: return UP;
        case DOWN:  return RIGHT;
        case LEFT:  return DOWN;
    }
}

Town::Direction Town::internal_corner_direction(const Direction& from)
{
    switch (from) {                
        case UP:    return RIGHT;
        case RIGHT: return DOWN;
        case DOWN:  return LEFT;
        case LEFT:  return UP;
    }
}

Town::Direction Town::flat_wall_direction(const Direction& from)
{
    switch (from) {
        case UP:    return DOWN;
        case RIGHT: return LEFT;
        case DOWN:  return UP;
        case LEFT:  return RIGHT;
    }
}

Town::Direction Town::orient(const Block& b, Direction& from)
{
    if (from == NONE)
        return first_move_direction(b);

    switch (scanner_view(b, from)) {
        case EXTERNAL_CORNER: return external_corner_direction(from);
        case INTERNAL_CORNER: return internal_corner_direction(from);
        case FLAT_WALL:       return flat_wall_direction(from);
    }
}

void Town::move(Scanner& s, Direction& to)
{
    Direction& from = to;
    switch (to) {
        case UP:    { s.move_up();    from = DOWN;  } break;
        case RIGHT: { s.move_right(); from = LEFT;  } break;
        case DOWN:  { s.move_down();  from = UP;    } break;
        case LEFT:  { s.move_left();  from = RIGHT; } break;
    }
}

void Town::move_clockwise(Scanner& s, Direction& from)
{
    Direction& to = from;
               to = orient(**s, from);
     move(s,   to);
}