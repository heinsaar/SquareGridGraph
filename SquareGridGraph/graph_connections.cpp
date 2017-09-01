#include "graph.h"

void Town::reset(int& depth)
{
    depth = 1;
}

bool Town::is_connected(ID id)
{
    return connected_.contains(id);
}

void Town::record_connection(const ID b_id)
{
    connected_.push_back(b_id);
    unvisited_.remove(b_id);
}

Town::Directions Town::seek_directions(const Block& b)
{
    Directions directions;

    if (!b.up)    directions.push_back(UP);
    if (!b.right) directions.push_back(RIGHT);
    if (!b.down)  directions.push_back(DOWN);
    if (!b.left)  directions.push_back(LEFT);

    return directions;
}

void Town::connect_buildings(BuildingPos a, BuildingPos b, const Direction ab)
{
//  display("\n Connecting buildings... ");
//  display((**a).b_id_);
//  if ((**a).b_id_ > 9) SGL_SPACE;
//  else DBL_SPACE;
//  display((**b).b_id_);

    const int length = grid_.distance(a, b);

    place_blocks_between(a, b, ab);
    build_bridge(a, length, ab);

    bridges_total_length_ += length;
    bridges_++;
}

void Town::place_blocks_between(BuildingPos a, BuildingPos b, const Direction ab)
{
    switch (ab) {
        case UP: {
            a.move_up();
            while (a != b) {
                if (is_free(*a))
                    put_block(*a);
                a.move_up();
            }
        } break;
        case RIGHT: {
            a.move_right();
            while (a != b) {
                if (is_free(*a))
                    put_block(*a);
                a.move_right();
            }
        } break;
        case DOWN: {
            a.move_down();
            while (a != b) {
                if (is_free(*a))
                    put_block(*a);
                a.move_down();
            }
        } break;
        case LEFT: {
            a.move_left();
            while (a != b) {
                if (is_free(*a))
                    put_block(*a);
                a.move_left();
            }
        } break;
    }
}

void Town::build_bridge(PanelPlacer a, const int length, const Direction from_a)
{
    PanelPlacer b = a;

    switch(from_a) {
    case UP: {
            b.move_up();
            for (int i = 0; i < length; ++i) {
                put_panel_at((**a).up);
                put_panel_at((**b).down);
                a.move_up(); b.move_up();
            }
        } break;
        case RIGHT: {
            b.move_right();
            for (int i = 0; i < length; ++i) {
                put_panel_at((**a).right);
                put_panel_at((**b).left);
                a.move_right(); b.move_right();
            }
        } break;
        case DOWN: {
            b.move_down();
            for (int i = 0; i < length; ++i) {
                put_panel_at((**a).down);
                put_panel_at((**b).up);
                a.move_down(); b.move_down();
            }
        } break;
        case LEFT: {
            b.move_left();
            for (int i = 0; i < length; ++i) {
                put_panel_at((**a).left);
                put_panel_at((**b).right);
                a.move_left(); b.move_left();
            }
        } break;
    }
}

bool Town::connect_isolated(const Walker& w, int depth)
{
    bool found = false;

    for (const auto& d : seek_directions(**w))
    {
        BuildingPos c; // closest at depth distance

        switch (d) {
            case UP: {
                c = seek_up(w, depth);
                if (c != w) // found?
                    if (found = !is_connected((**c).b_id_)) {
                        connect_buildings(w, c, UP);
                        record_connection((**c).b_id_);
                    }                        
            } break;
            case RIGHT: {
                c = seek_right(w, depth);
                if (c != w) // found?
                    if (found = !is_connected((**c).b_id_)) {
                        connect_buildings(w, c, RIGHT);
                        record_connection((**c).b_id_);
                    }
            } break;
            case DOWN: {
                c = seek_down(w, depth);
                if (c != w) // found?
                    if (found = !is_connected((**c).b_id_)) {
                        connect_buildings(w, c, DOWN);
                        record_connection((**c).b_id_);
                    }                        
            } break;
            case LEFT: {
                c = seek_left(w, depth);
                if (c != w) // found?
                    if (found = !is_connected((**c).b_id_)) {
                        connect_buildings(w, c, LEFT);
                        record_connection((**c).b_id_);
                    }
            } break;
        }
        if (found)
            return true;
    }
    return false; // did not find
}
