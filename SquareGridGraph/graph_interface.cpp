#include "graph.h"

using namespace sgg;

Town::Town(const HashDot& buildings)
    : h_d_(buildings), grid_(buildings.length() + 1, buildings.height() + 1)
{
    create_model(buildings);
    enumerate_buildings();
}

void Town::connect_all(bool say, bool viz)
{
    if (viz) display_hash_dot();
    if (say) display(" Connecting...");

    connect();

    if (say) display(" OK");

    if (viz)
    {
        display_model();
        display_statistics();
    }
}

void Town::display_model()
{
    ISOLATE;

    Walker s;

    NEW_LINE;
    for (int y = 0; y < grid_.max_y(); ++y) {
        grid_.locate(s, 0, y);
        SGL_SPACE;
        for (int x = 0; x < grid_.max_x(); ++x) {
            if (is_free(*s))
                display(" ");
            else if (is_on_building(s))
                display("o");
            else // is on a bridge
                display(".");
            s.move_right();
        }
        NEW_LINE;
    }
}

void Town::write_to(std::string fileName, bool withStats)
{
    std::ofstream file(fileName);
    if (!file)
        throw std::exception(("Error: Unable to open file " + quote(fileName) + ".").c_str());

    Walker s;

    for (int y = 0; y < grid_.max_y(); ++y) {
        grid_.locate(s, 0, y);
        file << " ";
        for (int x = 0; x < grid_.max_x(); ++x) {
            if (is_free(*s))
                file << " ";
            else if (is_on_building(s))
                file << "o";
            else // is on a bridge
                file << ".";
            s.move_right();
        }
        file << std::endl;
    }

    if (withStats)
    {
        file << "\n\n";
        file << " ";
        for (int j = 0; j < 23; ++j)
            file << "-";

        file << "\n Number of bridges....";
        file << bridges_;
        file << "\n Total length.........";
        file << bridges_total_length_;
        file << "\n Disconnected groups..";
        file << disconnected_groups_;

        if (bridges_ == 0) {
            file << "\n\n";
            if (disconnected_groups_ > 1)
                display(" No bridges are possible.");
            else
                display(" No bridges are needed.");
        }
        file << "\n";
        file << " ";
        for (int j = 0; j < 23; ++j)
            file << "-";
    }
}

void Town::display_statistics()
{
    ISOLATE;
    SGL_SPACE; border();

    display("\n Number of bridges....");
    display(bridges_);
    display("\n Total length.........");
    display(bridges_total_length_);
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
    SGL_SPACE; border();
    NEW_LINE;
}

void Town::display_hash_dot()
{
    ISOLATE;
    for (int n = 0; n < h_d_.height(); ++n) {
        SGL_SPACE; display(h_d_.get_line(n));
        NEW_LINE;
    }
    ISOLATE;
}