// Main.cpp

// INCLUDE
#include "Town.h"

// MAIN
int main()
{
    srand(time(0));

    /*	HashDot hashdot;
    hashdot.input_downtown_area();
    hashdot.input_hash_dot();
    */
    //	FOREVER
    DO
    {
        HashDot hashdot;

//		hashdot.random(120,45, 10);	// full screen beauty
		hashdot.random(10, 10, 5);	// middle size view
//      hashdot.random(30, 15, 4);	// small view

        Town town(hashdot);
        town.HashDotView();
        town.BuildBridges();
        town.ModelView();
        town.ViewStatistics();
        display("\n Next downtown...");
    }
    CONTINUE_UPON_REQUEST;
}

