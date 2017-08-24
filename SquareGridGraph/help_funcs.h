// HelpFuncs.h

#ifndef _HELP_FUNCTIONS_
#define _HELP_FUNCTIONS_

    // INCLUDE
#include <iostream>
#include <ctime>

    // DECLARATIONS
#define ISOLATE   (cout << endl << endl)
#define NEW_LINE  (cout << endl)
#define SGL_SPACE (cout << " ")
#define DBL_SPACE (cout << "  ")
#define FOREVER while (true)
#define DO do
#define CONTINUE_UPON_REQUEST while (cin.get() != '0')

using std::cout;
using std::endl;

namespace {

    template<class Info>
    inline void display(const Info info)
    {
        cout << info;
    }

    inline int difference(clock_t t1, clock_t t2) { return (int)(t2 - t1); }
    
    inline void border(const int& size)
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
} // NAMESPACE

#endif // _HELP_FUNCTIONS_