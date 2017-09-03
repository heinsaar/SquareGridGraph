#include "graph.h"

using namespace sgg;

struct Random { int X, Y, D; };

Random sec__5_d = { 150, 20, 40 }; // about 5  seconds in debug mode
Random sec_15_d = { 150, 40, 40 }; // about 12 seconds in debug mode

auto timepack = sec__5_d;

int X = timepack.X;
int Y = timepack.Y;

int main() try
{
    DO
    {
        HashDot hashdot;
        hashdot.random(timepack.X, timepack.Y, timepack.D);
        Town town(hashdot);
        town.connect_all(true, true);
    }
    CONTINUE_ON_ENTER
}
catch (const std::exception& e)
{
    display(e.what());
}