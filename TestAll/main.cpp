// Main.cpp

#include "../SquareGridGraph/graph.h"
#include <future>

using namespace sgg;

int X = 70;
int Y = 30;

const std::string dirHashdotGold        = "../../Tests/Hashdots_gold/";
const std::string dirModelConnected     = "../../Tests/Connected/";
const std::string dirModelConnectedGold = "../../Tests/Connected_gold/";

int test_range(int a, int b)
{
     static std::mutex m; // 44 seconds (all tests)

    int failedTestsInRange = 0;
    for (int i = a; i < b; i++)
    {
        HashDot hashdot;

        const std::string fileModelHashdot     = "model_hashdot_" + std::to_string(i) + ".txt";
        const std::string fileModelHashdotPath = dirHashdotGold + fileModelHashdot;
        hashdot.read_from(fileModelHashdotPath);

        Town town(hashdot); m.lock();   // 44 seconds (all tests)
        display_line(i);
        town.connect_all(); m.unlock(); // 44 seconds (all tests)

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
    auto f1 = std::async(std::launch::async, test_range,    0,  500);
    auto f2 = std::async(std::launch::async, test_range,  500, 1000);
    auto f3 = std::async(std::launch::async, test_range, 1000, 1500);
    auto f4 = std::async(std::launch::async, test_range, 1500, 2000);

    int failedTestsTotal = f1.get() + f2.get() + f3.get() + f4.get();

    display_line("-------------------");
    display_line(failedTestsTotal ? "FAIL: " + std::to_string(failedTestsTotal) + " tests." : "PASS: All tests.");
    display_line("-------------------\n");
}
catch (const std::exception& e)
{
    display(e.what());
}