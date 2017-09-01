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

Town::Directions Town::get_seek_directions(const Block& b)
{
    Directions directions;

    if (!b.up)    directions.push(UP);
    if (!b.right) directions.push(RIGHT);
    if (!b.down)  directions.push(DOWN);
    if (!b.left)  directions.push(LEFT);

    return directions;
}

void Town::connect_buildings(Buildingpos a, Buildingpos b, const Direction ab)
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

void Town::place_blocks_between(Buildingpos a, Buildingpos b, const Direction ab)
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

bool Town::connect_isolated(Walker s, int d)
{
    int& depth = d; // seek depth // TODO: Can probably be a member or something to avoid passing around.
    Directions directions = get_seek_directions(**s);

    bool found = false;

    while (!directions.empty()) {

        Buildingpos c; // closest at d distance

        switch (directions.front()) {
            case UP: {
                c = seek_up(s, depth);
                if (c != s) // found?
                    if (found = !is_connected((**c).b_id_)) {
                        connect_buildings(s, c, UP);
                        record_connection((**c).b_id_);
                    }                        
            } break;
            case RIGHT: {
                c = seek_right(s, depth);
                if (c != s) // found?
                    if (found = !is_connected((**c).b_id_)) {
                        connect_buildings(s, c, RIGHT);
                        record_connection((**c).b_id_);
                    }
            } break;
            case DOWN: {
                c = seek_down(s, depth);
                if (c != s) // found?
                    if (found = !is_connected((**c).b_id_)) {
                        connect_buildings(s, c, DOWN);
                        record_connection((**c).b_id_);
                    }                        
            } break;
            case LEFT: {
                c = seek_left(s, depth);
                if (c != s) // found?
                    if (found = !is_connected((**c).b_id_)) {
                        connect_buildings(s, c, LEFT);
                        record_connection((**c).b_id_);
                    }
            } break;
        }
        if (found)
            return true;  // found
        directions.pop(); // seek next...
    }
    return false; // did not find
}
