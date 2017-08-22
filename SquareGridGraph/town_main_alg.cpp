// Dtown_Main_Alg.cpp

#include "Town.h"

// THE CONNECTING ALGORITHM
void Town::_Connect_group(_Scanner s)
{
    _Record_connection((**s).b_id_);

    int dist = 1; // seek depth
    int max_depth = std::max(grid_.max_x(), grid_.max_y());

    _Direction from = NONE;	  // first move always from
    _Move_clockwise(s, from); // an EXTERNAL_CORNER
    _Scanner start = s;		  // from LEFT to RIGHT

    while (dist != max_depth) {
        do {
            if (_Is_on_building(s)) { // may be on a bridge!
                bool found = _Connect_isolated(s, dist);
                if (isolated_.empty())
                    return;
                if (found)
                    _Reset(dist); // reset seek depth
            }
            _Move_clockwise(s, from);
        } while (s != start);
        if (from == LEFT) // done full cycle
            ++dist;
    }
    connected_.clear();
}

// THE MAIN ALGORITHM
void Town::_Build_bridges()
{
    _Buildingptr b;
    while (!isolated_.empty()) {
        b = _Building_location(isolated_.front());
        _Connect_group(b);
        ++disconnected_groups_;
    }
}