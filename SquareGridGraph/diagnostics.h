#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <exception>
#include <string>

namespace sgg {

    enum class Diagnostic { FATAL_ERROR, ERROR };

    struct Error : std::exception
    {
        Error(const std::string& what) : std::exception(("Error: " + what).c_str()) {}
    };

} // NAMESPACE

#endif // DIAGNOSTICS_H