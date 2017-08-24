// Dtown_Model.cpp

#include "Town.h"

// CREATING THE DOWNTOWN MODEL
void Town::process_hash_dot(const HashDot& buildings)
{
    create_downtown_model(buildings);
}

bool Town::is_hash(const char& symbol)
{
    return symbol == '#';
}

void Town::set_building_id(Block& b, ID id)
{
    b.b_id_ = id;
}

void Town::put_block(BlockSite& s)
{
    s = new Block();
}

void Town::put_panel(PanelSite& s, Panel& pan)
{
    s = &pan;
}

void Town::put_panel_at(PanelSite& s, Panel& pan)
{
    if (is_free(s))
        put_panel(s, pan);
}

void Town::put_block_at(BlockSite& s)
{
    if (is_free(s))
        put_block(s);
}

void Town::place_blocks_clockwise(BlockPlacer b_p)
{
    put_block_at(*b_p); b_p.move_right();
    put_block_at(*b_p); b_p.move_down();
    put_block_at(*b_p); b_p.move_left();
    put_block_at(*b_p);
}

void Town::place_panels_clockwise(PanelPlacer p_p)
{
    Panel pan_1, pan_2, pan_3, pan_4;

    put_panel_at((*p_p)->down, pan_4);
    put_panel_at((*p_p)->right, pan_1);

    p_p.move_right();

    put_panel_at((*p_p)->left, pan_1);
    put_panel_at((*p_p)->down, pan_2);

    p_p.move_down();

    put_panel_at((*p_p)->up, pan_2);
    put_panel_at((*p_p)->left, pan_3);

    p_p.move_left();

    put_panel_at((*p_p)->right, pan_3);
    put_panel_at((*p_p)->up, pan_4);
}

void Town::create_downtown_model(const HashDot& buildings)
{
    BlockPlacer  b_p;
    PanelPlacer& p_p = b_p;

    for (int h = 0; h < buildings.height(); ++h) {
        grid_.locate(b_p, 0, h);
        string hd_str = buildings.getLine(h);
        for (Symbolptr s_p = hd_str.begin();
                        s_p < hd_str.end();
                      ++s_p , b_p.move_right()) {
            if (is_hash(*s_p)) {
                place_blocks_clockwise(b_p);
                place_panels_clockwise(p_p);
            }
        }
    }
}
