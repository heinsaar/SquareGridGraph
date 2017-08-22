// Main.cpp

// INCLUDE
#include "Town.h"

int X = 120;
int Y = 45;

// MAIN
int main()
{
    srand(time(0));

    /*	HashDot hashdot;
    hashdot.input_downtown_area();
    hashdot.input_hash_dot();
    */

    //	FOREVER
//    DO
    {
        HashDot hashdot;

		hashdot.random(X, Y, 10);

        Town town(hashdot);
        town.HashDotView();
        town.BuildBridges();
        town.ModelView();
        town.ViewStatistics();
//        display("\n Next downtown...");
    }
//    CONTINUE_UPON_REQUEST;
}

