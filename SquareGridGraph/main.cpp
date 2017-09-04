#include "graph.h"

using namespace sgg;

struct Random { int X, Y, D; };

Random d_sec_5  = { 150,  20, 40 }; // about  5 seconds in debug   mode
Random d_sec_15 = { 150,  40, 40 }; // about 12 seconds in debug   mode
Random r_sec_7  = { 200, 200, 40 }; // about  7 seconds in release mode
Random r_sec_28 = { 250, 250, 40 }; // about 28 seconds in release mode

auto timepack = r_sec_7;

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