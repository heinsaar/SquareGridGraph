#include "graph.h"

// THE CONNECTING ALGORITHM
void Town::connect_group(Walker w)
{
    record_connection(w->b_id_);

    int depth = 1; // seek depth
    int max_depth = std::max(grid_.max_x(), grid_.max_y());

    Direction from = Direction::NONE; // first move always from
    move_clockwise(w, from);          // an EXTERNAL_CORNER
    Walker start = w;                 // from LEFT to RIGHT

    while (depth != max_depth) {
        do {
            if (unvisited_.empty())
                return;               // we're done

            if (is_on_building(w))    // may be on a bridge!
            {
                bool found = connect_isolated(w, depth);
                if (found)
                    reset(depth);     // reset seek depth
            }
            move_clockwise(w, from);
        } while (w != start);

        if (from == Direction::LEFT)  // done full cycle
            depth++;
    }
    connected_.clear();
}

// THE MAIN ALGORITHM
void Town::connect()
{
    while (!unvisited_.empty()) {
        BuildingPos b = get_building_location(unvisited_.front());
        connect_group(b);
        stats_.disconnected_groups_++;
    }
}