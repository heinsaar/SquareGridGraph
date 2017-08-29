// Main.cpp

#include "Town.h"

int X = 70;
int Y = 30;

const std::string dirHashdotGold        = "../Tests/Hashdots_gold/";
const std::string dirModelConnected     = "../Tests/Connected/";
const std::string dirModelConnectedGold = "../Tests/Connected_gold/";

int main() try
{
    srand(time(0));

    // TODO: Parallelize tests.
    TIMES(100) // TODO: Unhardcode the number of tests.
    {
        HashDot hashdot;

        const std::string fileModelHashdot = "model_hashdot_" + std::to_string(i) + ".txt";
        hashdot.read_from(dirHashdotGold + fileModelHashdot);

        Town town(hashdot);
//      town.display_hash_dot();
        display_line(i);
        town.connect_all();
//      town.display_model();
//      town.display_statistics();

        const std::string fileModelConnected     = "model_connected_" + std::to_string(i) + ".txt";
        const std::string fileConnectedPath      = dirModelConnected     + fileModelConnected;
        const std::string fileModelConnectedGold = dirModelConnectedGold + fileModelConnected;

        town.write_to(fileConnectedPath);

        if (!files_identical(fileConnectedPath, fileModelConnectedGold))
            display_line("FAIL: Files " + quote(fileConnectedPath) + " and " + quote(fileModelConnectedGold) + " differ.");
    }
    display_line("Finished tests.");
}
catch (const std::exception& e)
{
    display(e.what());
}