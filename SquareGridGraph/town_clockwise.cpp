// Dtown_Clockwise.cpp

#include "Town.h"

// MOVING CLOCKWISE
Town::_Direction Town::_First_move_direction(const _Block& b)
{
    return _First_from_external_corner(b);
}

Town::_Direction Town::_First_from_external_corner(const _Block& b)
{
	// always one type of EXTERNAL_CORNER
	// (for the main algorithm using the
	// current _Building_location method)

		if (_Is_free(b.left) &&
			_Is_free(b.up) &&
			!_Is_free(b.right))
			return RIGHT;

	// other types of EXTERNAL_CORNER
	// (present here for extensibility)
/*		if (_Is_free(b.up) &&
			_Is_free(b.right) &&
			!_Is_free(b.down))
			return DOWN;
		if (_Is_free(b.right) &&
			_Is_free(b.down) &&
			!_Is_free(b.left))
			return LEFT;
		if (_Is_free(b.down) &&
			_Is_free(b.left) &&
			!_Is_free(b.up))
			return UP;			*/
}

Town::_Direction Town::_External_corner_direction(const _Direction& from)
{
	switch (from) {				
		case UP:	return LEFT;
		case RIGHT:	return UP;
		case DOWN:	return RIGHT;
		case LEFT:  return DOWN;
	}
}

Town::_Direction Town::_Internal_corner_direction(const _Direction& from)
{
	switch (from) {				
		case UP:	return RIGHT;
		case RIGHT:	return DOWN;
		case DOWN:	return LEFT;
		case LEFT:  return UP;
	}
}

Town::_Direction Town::_Flat_wall_direction(const _Direction& from)
{
	switch (from) {
		case UP:    return DOWN;
		case RIGHT: return LEFT;
		case DOWN:  return UP;
		case LEFT:  return RIGHT;
	}
}

Town::_Direction Town::_Orient(const _Block& b, _Direction& from)
{
	if (from == NONE)
		return _First_move_direction(b);

	switch (_Scanner_view(b, from)) {
		case EXTERNAL_CORNER: return _External_corner_direction(from);
		case INTERNAL_CORNER: return _Internal_corner_direction(from);
		case FLAT_WALL:       return _Flat_wall_direction(from);
	}
}

void Town::_Move(_Scanner& s, _Direction& to)
{
	_Direction& from = to;
	switch (to) {
		case UP:    { s.move_up();	  from = DOWN;  } break;
		case RIGHT: { s.move_right(); from = LEFT;  } break;
		case DOWN:  { s.move_down();  from = UP;    } break;
		case LEFT:  { s.move_left();  from = RIGHT; } break;
	}
}

void Town::_Move_clockwise(_Scanner& s, _Direction& from)
{
	_Direction& to = from;
				to = _Orient(**s, from);
	 _Move(s,   to);
}