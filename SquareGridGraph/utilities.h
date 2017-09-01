// HelpFuncs.h

#ifndef _HELP_FUNCTIONS_
#define _HELP_FUNCTIONS_

    // INCLUDE
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <string>
#include <ctime>

    // DECLARATIONS
#define ISOLATE   (std::cout << std::endl << std::endl)
#define NEW_LINE  (std::cout << std::endl)
#define SGL_SPACE (std::cout << " ")
#define DBL_SPACE (std::cout << "  ")
#define FOREVER while (true)
#define DO do
#define CONTINUE_UPON_REQUEST while (std::cin.get() != '0');
#define TIMES(N) for (int i=0; i<(N); i++)

namespace sgg {

    // TODO: Create a thread-safe display class or function.

    inline std::string quote(std::string s) { return "\"" + s + "\""; }

    int rand(int min, int max, bool seed = true);

    template<class Displayable>
    inline void display(const Displayable info)
    {
        std::cout << info;
    }

    template<class Displayable>
    inline void display_line(const Displayable info)
    {
        std::cout << std::endl << info;
    }
    
    inline int difference(clock_t t1, clock_t t2) { return (int)(t2 - t1); }
    
    inline void border(int size = 23)
    {
        for (int j = 0; j < size; ++j)
            display("-");
    }

    inline void show_duration(clock_t t1, clock_t t2)
    {
        ISOLATE;
        display("Execution time: ");
        display(difference(t1, t2));
        ISOLATE;
    }

    bool files_identical(std::string fileNameA, std::string fileNameB);

} // NAMESPACE

#endif // _HELP_FUNCTIONS_