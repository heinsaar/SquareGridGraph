// Main.cpp

#include "graph.h"

using namespace sgg;

int X = 150;
int Y = 20;

int main() try
{
    DO
    {
        HashDot hashdot;
        hashdot.random(X, Y, 40);
        Town town(hashdot);
        town.connect_all(true, true);
    }
    CONTINUE_UPON_REQUEST
}
catch (const std::exception& e)
{
    display(e.what());
}