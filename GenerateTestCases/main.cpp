// Main.cpp

// INCLUDE
#include "../SquareGridGraph/town.h"

#include <fstream>
#include <sstream>
#include <thread>
#include <mutex>
#include <atomic>

int X = 70;
int Y = 30;

std::atomic<int> n;
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
    srand(time(0));

    std::thread t1(create_examples, 500);
    std::thread t2(create_examples, 500);
    std::thread t3(create_examples, 500);
    std::thread t4(create_examples, 500);
    
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}
catch (const std::exception& e)
{
    display(e.what());
}

