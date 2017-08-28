// Main.cpp

// INCLUDE
#include "Town.h"

#include <fstream>
#include <sstream>

int X = 70;
int Y = 30;

int main() try
{
    srand(time(0));

    DO
    {
        HashDot hashdot;

        hashdot.random(X, Y, 40);
//      hashdot.read_from("town.txt");

        Town town(hashdot);
        town.display_hash_dot();
        town.connect_all();
        town.display_model();
        town.display_statistics();
        
        display("\n Next downtown... \n\n");
    }
    CONTINUE_UPON_REQUEST;
}
catch (const std::exception& e)
{
    display(e.what());
}

