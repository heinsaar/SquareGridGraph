// Dtown_Model.cpp

#include "Town.h"

// CREATING THE DOWNTOWN MODEL
void Town::_Process_hash_dot(const HashDot& buildings)
{
    _Create_downtown_model(buildings);
}

bool Town::_Is_hash(const char& symbol)
{
    return symbol == '#';
}

void Town::_Set_building_id(_Block& b, _ID id)
{
    b.b_id_ = id;
}

void Town::_Put_a_block(_BlockSite& s)
{
    s = new _Block();
}

void Town::_Put_panel(_PanelSite& s, _Panel& pan)
{
    s = &pan;
}

void Town::_Place_panel_at(_PanelSite& s, _Panel& pan)
{
	if (_Is_free(s))
		_Put_panel(s, pan);
}

void Town::_Place_block_at(_BlockSite& s)
{
	if (_Is_free(s))
		_Put_a_block(s);
}

void Town::_Place_blocks_clockwise(_BlockPlacer b_p)
{
	_Place_block_at(*b_p); b_p.move_right();
	_Place_block_at(*b_p); b_p.move_down();
	_Place_block_at(*b_p); b_p.move_left();
	_Place_block_at(*b_p);
}

void Town::_Place_panels_clockwise(_PanelPlacer p_p)
{
	_Panel pan_1, pan_2, pan_3, pan_4;

	_Place_panel_at((*p_p)->down, pan_4);
	_Place_panel_at((*p_p)->right, pan_1);

	p_p.move_right();

	_Place_panel_at((*p_p)->left, pan_1);
	_Place_panel_at((*p_p)->down, pan_2);

	p_p.move_down();

	_Place_panel_at((*p_p)->up, pan_2);
	_Place_panel_at((*p_p)->left, pan_3);

	p_p.move_left();

	_Place_panel_at((*p_p)->right, pan_3);
	_Place_panel_at((*p_p)->up, pan_4);
}

void Town::_Create_downtown_model(const HashDot& buildings)
{
	_BlockPlacer  b_p;
	_PanelPlacer& p_p = b_p;

	for (int h = 0; h < buildings.height(); ++h) {
		grid_.locate(b_p, 0, h);
		string hd_str = buildings.get_line(h);
		for (_Symbolptr s_p = hd_str.begin();
						s_p < hd_str.end();
					  ++s_p , b_p.move_right()) {
			if (_Is_hash(*s_p)) {
				_Place_blocks_clockwise(b_p);
				_Place_panels_clockwise(p_p);
			}
		}
	}
}
