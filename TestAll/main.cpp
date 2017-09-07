// Main.cpp

#include "../SquareGridGraph/graph.h"

#include <future>
#include <chrono>

using namespace sgg;

static std::exception_ptr threx = nullptr;

const std::string dirHDGold        = "../../Tests/Hashdots_gold/";
const std::string dirConnected     = "../../Tests/Connected/";
const std::string dirConnectedGold = "../../Tests/Connected_gold/";

int test_range(int a, int b) try
{
    static std::mutex m; // 44 seconds (all tests)

    int failedTestsInRange = 0;
    for (int i = a; i < b; i++)
    {
        HashDot hashdot;

        const std::string fileModelHashdot     = "model_hashdot_" + std::to_string(i) + ".txt";
        const std::string fileModelHashdotPath = dirHDGold + fileModelHashdot;
        hashdot.read_from(fileModelHashdotPath);

        Town town(hashdot); m.lock();   // 44 seconds (all tests)
        display_line(i);
        town.connect_all(); m.unlock(); // 44 seconds (all tests)

        const std::string fileConnected     = "model_connected_" + std::to_string(i) + ".txt";
        const std::string fileConnectedPath = dirConnected       + fileConnected;
        const std::string fileConnectedGold = dirConnectedGold   + fileConnected;

        town.write_to(fileConnectedPath);

        if (!files_identical(fileConnectedPath, fileConnectedGold))
        {
            display_line("FAIL: Files " + quote(fileConnectedPath) + " and " + quote(fileConnectedGold) + " differ.");
            failedTestsInRange++;
        }
    }
    return failedTestsInRange;
}
catch (...)
{
    threx = std::current_exception();
}

int main() try
{
    DO
    {{
        Timer<> time("All tests"); // Tests duration improvement (seconds): 44 > 26

        auto f1 = std::async(std::launch::async, test_range,    0,  500);
        auto f2 = std::async(std::launch::async, test_range,  500, 1000);
        auto f3 = std::async(std::launch::async, test_range, 1000, 1500);
        auto f4 = std::async(std::launch::async, test_range, 1500, 2000);

        int totalFails = f1.get() + f2.get() + f3.get() + f4.get();

        if (threx) std::rethrow_exception(threx);
                
        display_line("-------------------");
        display_line(totalFails ? "FAIL: " + std::to_string(totalFails) + " tests." : "PASS: All tests.");
        display_line("-------------------\n");
    }}
    CONTINUE_ON_ENTER
}
catch (const std::exception& e)
{
    display(e.what());
}