// Main.cpp

#include "graph.h"

int X = 70;
int Y = 30;

const std::string dirHashdotGold        = "../Tests/Hashdots_gold/";
const std::string dirModelConnected     = "../Tests/Connected/";
const std::string dirModelConnectedGold = "../Tests/Connected_gold/";

int main() try
{
    srand(time(0));

    int failedTests = 0;

    // TODO: Parallelize tests.
    for (int i = 0; i < 50; i++) // TODO: Unhardcode the number of tests.
    {
        HashDot hashdot;

        const std::string fileModelHashdot     = "model_hashdot_" + std::to_string(i) + ".txt";
        const std::string fileModelHashdotPath = dirHashdotGold + fileModelHashdot;
        hashdot.read_from(fileModelHashdotPath);

        Town town(hashdot);
        display_line(i);
        town.connect_all();

        const std::string fileModelConnected     = "model_connected_" + std::to_string(i) + ".txt";
        const std::string fileConnectedPath      = dirModelConnected     + fileModelConnected;
        const std::string fileModelConnectedGold = dirModelConnectedGold + fileModelConnected;

        town.write_to(fileConnectedPath);

        if (!files_identical(fileConnectedPath, fileModelConnectedGold))
        {
            display_line("FAIL: Files " + quote(fileConnectedPath) + " and " + quote(fileModelConnectedGold) + " differ.");
            failedTests++;
        }
    }
    display_line("-------------------");
    display_line(failedTests ? "FAIL: " + std::to_string(failedTests) + " tests." : "PASS: All tests.");
    display_line("-------------------\n");
}
catch (const std::exception& e)
{
    display(e.what());
}