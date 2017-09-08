#include "graph.h"

// CREATING THE MODEL

bool Town::is_hash(const char c)            { return c == '#'; }
void Town::set_building_id(Block& b, ID id) { b.b_id_ = id;    }
void Town::put_block(   BlockSite& s)       { s.used = true;   }
void Town::put_panel_at(PanelSite& s)       { s = true;        }

void Town::put_block_at(BlockSite& s)
{
    if (is_free(s))
        put_block(s);
}

void Town::place_blocks_clockwise(Walker w)
{
    put_block_at(*w); w.move_right();
    put_block_at(*w); w.move_down();
    put_block_at(*w); w.move_left();
    put_block_at(*w);
}

void Town::place_panels_clockwise(Walker w)
{
    put_panel_at(w->down);
    put_panel_at(w->right);

    w.move_right();

    put_panel_at(w->left);
    put_panel_at(w->down);

    w.move_down();

    put_panel_at(w->up);
    put_panel_at(w->left);

    w.move_left();

    put_panel_at(w->right);
    put_panel_at(w->up);
}

void Town::create_model(const HashDot& buildings)
{
    for (int n = 0; n < buildings.height(); ++n)
    {
        Walker w = grid_.cell(0, n);
        const std::string line = buildings.get_line(n);

        for (const auto& c : line)
        {
            if (is_hash(c)) {
                place_blocks_clockwise(w);
                place_panels_clockwise(w);
            }
            w.move_right();
        }
    }
}
