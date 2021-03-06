// Main.cpp

#include "../SquareGridGraph/graph.h"

#include <fstream>
#include <sstream>
#include <future>
#include <mutex>

using namespace sgg;

static std::exception_ptr threx = nullptr;

int n = 0;
std::mutex m;

void create_examples(int N) try
{
    TIMES(N)
    {
        std::lock_guard<std::mutex> guard(m);

        HashDot hashdot;

        const int X = 70;
        const int Y = 30;
        const int D = rand(0, 2); // density

        switch (D)
        {
        case 0:  hashdot.random(X, Y,  10); break; 
        case 1:  hashdot.random(X, Y,  40); break;
        case 2:  hashdot.random(X, Y, 100); break;
        default: hashdot.random(X, Y,  70); break;
        }

        hashdot.write_to("../Tests/Hashdots/model_hashdot_" + std::to_string(n) + ".txt");

        Town town(hashdot);
        town.connect_all();

        town.write_to("../Tests/Connected/model_connected_" + std::to_string(n) + ".txt");

        n++;
    }
}
catch (...)
{
    threx = std::current_exception();
}

int main() try
{
    std::async(std::launch::async, create_examples, 500);
    std::async(std::launch::async, create_examples, 500);
    std::async(std::launch::async, create_examples, 500);
    std::async(std::launch::async, create_examples, 500);

    if (threx) std::rethrow_exception(threx);
}
catch (const std::exception& e)
{
    display(e.what());
}

