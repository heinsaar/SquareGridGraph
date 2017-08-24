// Main.cpp

// INCLUDE
#include "Town.h"

#include <fstream>
#include <sstream>

int X = 70;
int Y = 30;

bool files_identical(std::string fileNameA, std::string fileNameB)
{
    std::ifstream A(fileNameA);
    std::ifstream B(fileNameB);

    std::stringstream a;
    std::stringstream b;

    a << A.rdbuf();
    b << B.rdbuf();

    return a.str() == b.str();
}

int main() try
{
    srand(time(0));

//  DO
    {
        HashDot hashdot;

//      hashdot.random(X, Y, 10);
        hashdot.read_from("town.txt");

        Town town(hashdot);
        town.hash_dot_view();
        town.connect_all();
        town.model_view();
        town.view_statistics();
        display("\n Next downtown... \n\n");
    }
//    CONTINUE_UPON_REQUEST;
}
catch (const std::exception& e)
{
    display(e.what());
}

