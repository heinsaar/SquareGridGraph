// Main.cpp

#include "../SquareGridGraph/graph.h"
#include <future>

using namespace sgg;

int X = 70;
int Y = 30;

const std::string dirHashdotGold        = "../Tests/Hashdots_gold/";
const std::string dirModelConnected     = "../Tests/Connected/";
const std::string dirModelConnectedGold = "../Tests/Connected_gold/";

int test_range(int a, int b)
{
    int failedTestsInRange = 0;
    for (int i = a; i < b; i++)
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
            failedTestsInRange++;
        }
    }
    return failedTestsInRange;
}

int main() try
{
    auto failedInRange1 = std::async(std::launch::async, test_range,    0,  500);
    auto failedInRange2 = std::async(std::launch::async, test_range,  500, 1000);
    auto failedInRange3 = std::async(std::launch::async, test_range, 1000, 1500);
    auto failedInRange4 = std::async(std::launch::async, test_range, 1500, 2000);

    int failedTestsTotal = failedInRange1.get()
                         + failedInRange2.get()
                         + failedInRange3.get()
                         + failedInRange4.get();

    display_line("-------------------");
    display_line(failedTestsTotal ? "FAIL: " + std::to_string(failedTestsTotal) + " tests." : "PASS: All tests.");
    display_line("-------------------\n");
}
catch (const std::exception& e)
{
    display(e.what());
}