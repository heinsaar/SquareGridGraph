// Main.cpp

#include "../SquareGridGraph/graph.h"

#include <fstream>
#include <sstream>
#include <future>
#include <mutex>

using namespace sgg;

int X = 70;
int Y = 30;

int n = 0;
std::mutex m;

void create_examples(int N)
{
    TIMES(N)
    {
        std::lock_guard<std::mutex> guard(m);

        HashDot hashdot;

        const int density = rand(0, 2);

        switch (density)
        {
        case 0: hashdot.random(X, Y,  10); break;
        case 1: hashdot.random(X, Y,  40); break;
        case 2: hashdot.random(X, Y, 100); break;
        }

        hashdot.write_to("../Tests/Hashdots/model_hashdot_" + std::to_string(n) + ".txt");

        Town town(hashdot);
        town.connect_all();

        town.write_to("../Tests/Connected/model_connected_" + std::to_string(n) + ".txt");

        n++;
    }
}

int main() try
{
    std::async(std::launch::async, create_examples, 500);
    std::async(std::launch::async, create_examples, 500);
    std::async(std::launch::async, create_examples, 500);
    std::async(std::launch::async, create_examples, 500);
}
catch (const std::exception& e) // TODO: Catch exceptions from threads also.
{
    display(e.what());
}

