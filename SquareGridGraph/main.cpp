#include "graph.h"

using namespace sgg;

struct Random { int X, Y, D; };

Random sec_5_d  = { 150, 20, 40 }; // about 5  seconds in debug mode
Random sec_15_d = { 150, 40, 40 }; // about 12 seconds in debug mode

auto package = sec_5_d;

int X = package.X;
int Y = package.Y;

int main() try
{
    DO
    {
        HashDot hashdot;
        hashdot.random(package.X, package.Y, package.D);
        Town town(hashdot);
        town.connect_all(true, true);
    }
    CONTINUE_ON_ENTER
}
catch (const std::exception& e)
{
    display(e.what());
}