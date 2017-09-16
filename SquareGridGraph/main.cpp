#include "graph.h"

using namespace sgg;

struct Random { int X, Y, D; std::string name; };

Random tiny     = {   5,   5, 40, "tiny"     }; // instantaneous
Random d_sec_1  = {  50,  20, 40, "d_sec_1"  }; // fast (with some work)
Random d_sec_5  = { 150,  20, 40, "d_sec_5"  }; // about  5 sec in debug   mode >  3 sec
Random d_sec_15 = { 150,  40, 40, "d_sec_15" }; // about 15 sec in debug   mode > 12 sec
Random r_sec_7  = { 200, 200, 40, "r_sec_7"  }; // about  7 sec in release mode >  6 sec (no viz)
Random r_sec_28 = { 250, 250, 40, "r_sec_28" }; // about 28 sec in release mode > 25 sec

#ifdef _DEBUG
Random timepack = d_sec_5;
#else
Random timepack = r_sec_7;
#endif

int main() try
{
    DO
    {
        HashDot hashdot;
        hashdot.random(timepack.X, timepack.Y, timepack.D);
//      hashdot.read_from("town_fast_seek.txt");
        Town town(hashdot);
        Timer<> t(timepack.name);
        town.connect_all();// true, true);
    }
    CONTINUE_ON_ENTER
}
catch (const std::exception& e)
{
    display(e.what());
}