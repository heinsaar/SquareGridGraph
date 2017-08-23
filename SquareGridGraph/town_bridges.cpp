// Dtown_Bridges.cpp

#include "Town.h"

#include <algorithm> // find()

// BUILDING BRIDGES
void Town::_Reset(int& depth)
{
    depth = 1;
}

bool Town::_Is_among(const list<_ID>& c, _ID id)
{
    return (std::find(c.begin(), c.end(), id) != c.end());
}

bool Town::_Is_connected(_ID id)
{
    return _Is_among(connected_, id);
}

void Town::_Record_connection(const _ID& b_id)
{
	connected_.push_back(b_id);
	unvisited_.remove(b_id);
}

void Town::_Set_seek_directions(_Directions& dirs, const _Block& b)
{
	if (_Is_free(b.up))	   dirs.push(UP);
	if (_Is_free(b.right)) dirs.push(RIGHT);
	if (_Is_free(b.down))  dirs.push(DOWN);
	if (_Is_free(b.left))  dirs.push(LEFT);
}

void Town::_Connect_buildings(_Buildingptr a, _Buildingptr b, const _Direction& ab)
{
//  display("\n Connecting buildings... ");
//  display((**a).b_id_);
//  if ((**a).b_id_ > 9) SGL_SPACE
//  else DBL_SPACE
//  display((**b).b_id_);
	int n = grid_.distance(a, b);

	vector<_Panel> panels(n);

	_Place_blocks_between(a, b, ab);
	_Build_bridge(a, panels, ab);

	total_bridges_length_ += n; ++bridges_;
}

void Town::_Place_blocks_between(_Buildingptr a, _Buildingptr b, const _Direction& ab)
{
	switch (ab) {
		case UP: {
			a.move_up();
			while (a != b) {
				if (_Is_free(*a))
					_Put_a_block(*a);
				a.move_up();
			}
		} break;
		case RIGHT: {
			a.move_right();
			while (a != b) {
				if (_Is_free(*a))
					_Put_a_block(*a);
				a.move_right();
			}
		} break;
		case DOWN: {
			a.move_down();
			while (a != b) {
				if (_Is_free(*a))
					_Put_a_block(*a);
				a.move_down();
			}
		} break;
		case LEFT: {
			a.move_left();
			while (a != b) {
				if (_Is_free(*a))
					_Put_a_block(*a);
				a.move_left();
			}
		} break;
	}
}

void Town::_Build_bridge(_PanelPlacer a, vector<_Panel>& pan, const _Direction& from_a)
{
	_PanelPlacer b = a;

	switch(from_a) {
	case UP: {
			b.move_up();
			for (int i = 0; i < pan.size(); ++i) {
				_Put_panel((**a).up, pan[i]);
				_Put_panel((**b).down, pan[i]);
				a.move_up(); b.move_up();
			}
		} break;
		case RIGHT: {
			b.move_right();
			for (int i = 0; i < pan.size(); ++i) {
				_Put_panel((**a).right, pan[i]);
				_Put_panel((**b).left, pan[i]);
				a.move_right(); b.move_right();
			}
		} break;
		case DOWN: {
			b.move_down();
			for (int i = 0; i < pan.size(); ++i) {
				_Put_panel((**a).down, pan[i]);
				_Put_panel((**b).up, pan[i]);
				a.move_down(); b.move_down();
			}
		} break;
		case LEFT: {
			b.move_left();
			for (int i = 0; i < pan.size(); ++i) {
				_Put_panel((**a).left, pan[i]);
				_Put_panel((**b).right, pan[i]);
				a.move_left(); b.move_left();
			}
		} break;
	}
}

bool Town::_Connect_isolated(_Scanner s, int d)
{
	int& depth = d; // seek depth
	_Directions dirs;
	_Set_seek_directions(dirs, **s);

	bool found = false;

	while (!dirs.empty()) {

		_Buildingptr c; // closest at d distance

		switch (dirs.front()) {
			case UP: {
				c = _Seek_up(s, depth);
				if (c != s) // found?
					if (found = !_Is_connected((**c).b_id_)) {
						_Connect_buildings(s, c, UP);
						_Record_connection((**c).b_id_);
					}						
			} break;
			case RIGHT: {
				c = _Seek_right(s, depth);
				if (c != s) // found?
					if (found = !_Is_connected((**c).b_id_)) {
						_Connect_buildings(s, c, RIGHT);
						_Record_connection((**c).b_id_);
					}
			} break;
			case DOWN: {
				c = _Seek_down(s, depth);
				if (c != s) // found?
					if (found = !_Is_connected((**c).b_id_)) {
						_Connect_buildings(s, c, DOWN);
						_Record_connection((**c).b_id_);
					}						
			} break;
			case LEFT: {
				c = _Seek_left(s, depth);
				if (c != s) // found?
					if (found = !_Is_connected((**c).b_id_)) {
						_Connect_buildings(s, c, LEFT);
						_Record_connection((**c).b_id_);
					}
			} break;
		}
		if (found)
			return true; // found!
		dirs.pop(); // seek next...
	}
	return false; // did not find
}
