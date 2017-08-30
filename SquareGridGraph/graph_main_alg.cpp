#include "graph.h"

// THE CONNECTING ALGORITHM
void Town::connect_group(Scanner s)
{
    record_connection((**s).b_id_);

    int depth = 1; // seek depth
    int max_depth = std::max(grid_.max_x(), grid_.max_y());

    Direction from = NONE;   // first move always from
    move_clockwise(s, from); // an EXTERNAL_CORNER
    Scanner start = s;       // from LEFT to RIGHT

    while (depth != max_depth) {
        do {
            if (is_on_building(s)) { // may be on a bridge!
                bool found = connect_isolated(s, depth);
                if (unvisited_.empty())
                    return;
                if (found)
                    reset(depth); // reset seek depth
            }
            move_clockwise(s, from);
        } while (s != start);

        if (from == LEFT) // done full cycle
            ++depth;
    }
    connected_.clear();
}

// THE MAIN ALGORITHM
void Town::connect()
{
    while (!unvisited_.empty()) {
        Buildingpos b = get_building_location(unvisited_.front());
        connect_group(b);
        ++disconnected_groups_;
    }
}