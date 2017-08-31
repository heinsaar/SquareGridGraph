// Main.cpp

#include "../SquareGridGraph/graph.h"

using namespace sgg;

int X = 70;
int Y = 30;

const std::string dirHashdotGold        = "../Tests/Hashdots_gold/";
const std::string dirModelConnected     = "../Tests/Connected/";
const std::string dirModelConnectedGold = "../Tests/Connected_gold/";

int main() try
{
    int failedTests = 0;

    // TODO: Parallelize tests.
    int from = rand(0, 1950);
    for (int i = from; i < from + 50; i++) // TODO: Unhardcode the number of tests.
//    TIMES(2000)
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