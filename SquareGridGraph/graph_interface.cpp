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

    NEW_LINE;
    for (int y = 0; y < grid_.max_y(); ++y) {
        Walker s = grid_.cell(0, y);
        SGL_SPACE;
        for (int x = 0; x < grid_.max_x(); ++x) {
            if (is_free(*s))            display(" ");
            else if (is_on_building(s)) display("o");
            else  /* is on bridge */    display(".");
            s.move_right();
        }
        NEW_LINE;
    }
}

void Town::write_to(std::string fileName, bool withStats)
{
    std::ofstream file(fileName);
    if (!file)
        throw Error("Unable to open file " + quote(fileName) + ".");

    for (int y = 0; y < grid_.max_y(); ++y) {
        Walker s = grid_.cell(0, y);
        file << " ";
        for (int x = 0; x < grid_.max_x(); ++x) {
            if (is_free(*s))            file << " ";
            else if (is_on_building(s)) file << "o";
            else  /* is on bridge */    file << ".";
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

        file << "\n Number of bridges...." << stats_.bridges_;
        file << "\n Total length........." << stats_.bridges_total_length_;
        file << "\n Disconnected groups.." << stats_.disconnected_groups_;

        if (stats_.bridges_ == 0) {
            file << "\n\n";
            if (stats_.disconnected_groups_ > 1)
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

    display(" Number of bridges....", stats_.bridges_);
    display(" Total length.........", stats_.bridges_total_length_);
    display(" Disconnected groups..", stats_.disconnected_groups_);

    if (stats_.bridges_ == 0) {
        ISOLATE;
        if (stats_.disconnected_groups_ > 1)
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