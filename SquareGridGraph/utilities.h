#ifndef UTILITIES_H
#define UTILITIES_H

    // INCLUDE
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <string>
#include <mutex>

#define ISOLATE   display("\n")
#define NEW_LINE  display("\n")
#define SGL_SPACE display(" ")
#define DBL_SPACE display("  ")
#define FOREVER while (true)
#define DO do
#define CONTINUE_ON_ENTER while (std::cin.get() != '0');
#define TIMES(N) for (int i=0; i<(N); i++)

namespace sgg {

    struct cout_synchronized : std::ostringstream
    {
         cout_synchronized() = default;
        ~cout_synchronized()
        {
            std::lock_guard<std::mutex> guard(mutexPrint_);
            std::cout << this->str();
        }

    private:
        static std::mutex mutexPrint_;
    };

    inline std::string quote(std::string s) { return "\"" + s + "\""; }

    int rand(int min, int max, bool seed = true);

    template<class Displayable>
    inline void display(const Displayable info)
    {
        cout_synchronized() << info;
    }

    template<class Displayable>
    inline void display_line(const Displayable info)
    {
        cout_synchronized() << std::endl << info;
    }
    
    inline void border(int size = 23)
    {
        for (int j = 0; j < size; ++j)
            display("-");
    }

    bool files_identical(std::string fileNameA, std::string fileNameB);

} // NAMESPACE

#endif // UTILITIES_H