// Dtown_Bridges.cpp

#include "graph.h"

#include <algorithm> // find()

// BUILDING BRIDGES
void Town::reset(int& depth)
{
    depth = 1;
}

bool Town::is_among(const std::list<ID>& c, ID id)
{
    return (std::find(c.begin(), c.end(), id) != c.end());
}

bool Town::is_connected(ID id)
{
    return is_among(connected_, id);
}

void Town::record_connection(const ID& b_id)
{
    connected_.push_back(b_id);
    unvisited_.remove(b_id);
}

void Town::set_seek_directions(Directions& dirs, const Block& b)
{
    if (is_free(b.up))    dirs.push(UP);
    if (is_free(b.right)) dirs.push(RIGHT);
    if (is_free(b.down))  dirs.push(DOWN);
    if (is_free(b.left))  dirs.push(LEFT);
}

void Town::connect_buildings(Buildingpos a, Buildingpos b, const Direction& ab)
{
//  display("\n Connecting buildings... ");
//  display((**a).b_id_);
//  if ((**a).b_id_ > 9) SGL_SPACE;
//  else DBL_SPACE;
//  display((**b).b_id_);

    int n = grid_.distance(a, b);

    vector<Panel> panels(n);

    place_blocks_between(a, b, ab);
    build_bridge(a, panels, ab);

    bridges_total_length_ += n; ++bridges_;
}

void Town::place_blocks_between(Buildingpos a, Buildingpos b, const Direction& ab)
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

void Town::build_bridge(PanelPlacer a, vector<Panel>& pan, const Direction& from_a)
{
    PanelPlacer b = a;

    switch(from_a) {
    case UP: {
            b.move_up();
            for (int i = 0; i < pan.size(); ++i) {
                put_panel((**a).up, pan[i]);
                put_panel((**b).down, pan[i]);
                a.move_up(); b.move_up();
            }
        } break;
        case RIGHT: {
            b.move_right();
            for (int i = 0; i < pan.size(); ++i) {
                put_panel((**a).right, pan[i]);
                put_panel((**b).left, pan[i]);
                a.move_right(); b.move_right();
            }
        } break;
        case DOWN: {
            b.move_down();
            for (int i = 0; i < pan.size(); ++i) {
                put_panel((**a).down, pan[i]);
                put_panel((**b).up, pan[i]);
                a.move_down(); b.move_down();
            }
        } break;
        case LEFT: {
            b.move_left();
            for (int i = 0; i < pan.size(); ++i) {
                put_panel((**a).left, pan[i]);
                put_panel((**b).right, pan[i]);
                a.move_left(); b.move_left();
            }
        } break;
    }
}

bool Town::connect_isolated(Scanner s, int d)
{
    int& depth = d; // seek depth // TODO: Can probably be a member or something to avoid passing around.
    Directions directions;
    set_seek_directions(directions, **s);

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
            return true; // found!
        directions.pop(); // seek next...
    }
    return false; // did not find
}
