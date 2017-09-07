#include "graph.h"

using namespace sgg;

struct Random { int X, Y, D; std::string name; };

Random d_sec_5  = { 150,  20, 40, "d_sec_5"  }; // about  5 sec in debug   mode >  3 sec
Random d_sec_15 = { 150,  40, 40, "d_sec_15" }; // about 12 sec in debug   mode > 12 sec
Random r_sec_7  = { 200, 200, 40, "r_sec_7"  }; // about  7 sec in release mode >  6 sec
Random r_sec_28 = { 250, 250, 40, "r_sec_28" }; // about 28 sec in release mode > 25 sec

auto timepack = d_sec_5;

int main() try
{
    DO
    {
        HashDot hashdot;
        hashdot.random(timepack.X, timepack.Y, timepack.D);
        Town town(hashdot);
        Timer<> t(timepack.name);
        town.connect_all(true, true);
    }
    CONTINUE_ON_ENTER
}
catch (const std::exception& e)
{
    display(e.what());
}