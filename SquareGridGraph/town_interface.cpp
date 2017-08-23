// Dtown_Interface.cpp

#include "Town.h"

Town::Town(HashDot buildings) : grid_(buildings.length() + 1, buildings.height() + 1)
{
    _Process_hash_dot(buildings);
    _Enumerate_buildings();

    h_d_ = buildings;

    total_bridges_length_ = 0;
    disconnected_groups_ = 0;
    bridges_ = 0;
}
void Town::BuildBridges()
{
    display(" Building bridges...");
    _Build_bridges();
}

// INTERFACE
void Town::ModelView()
{
    ISOLATE;

    _Scanner s;

    NEW_LINE;
    for (int y = 0; y < grid_.max_y(); ++y) {
        grid_.locate(s, 0, y); SGL_SPACE;
        for (int x = 0; x < grid_.max_x(); ++x) {
            if (_Is_free(*s))
                display(" ");
            else if (_Is_on_building(s))
                //	display((**s).b_id_);
                display("o");
            else // is on a bridge
                display(".");
            s.move_right();
        }
        NEW_LINE;
    }
}

void Town::ViewStatistics()
{
    ISOLATE;
    SGL_SPACE; border(23);

    display("\n Number of bridges....");
    display(bridges_);
    display("\n Total length.........");
    display(total_bridges_length_);
    display("\n Disconnected groups..");
    display(disconnected_groups_);

    if (bridges_ == 0) {
        ISOLATE;
        if (disconnected_groups_ > 1)
            display(" No bridges are possible.");
        else
            display(" No bridges are needed.");
    }
    NEW_LINE;
    SGL_SPACE; border(23);
    NEW_LINE;
}

void Town::HashDotView()
{
    ISOLATE;
    for (int n = 0; n < h_d_.height(); ++n) {
        SGL_SPACE; display(h_d_.getLine(n));
        NEW_LINE;
    }
    ISOLATE;
}