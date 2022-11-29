#ifndef __EASYLUA_EXCEPTION_H
#define __EASYLUA_EXCEPTION_H

#include <stdexcept>
#include <string>

namespace easylua
{
    class runtime_error : public std::runtime_error
    {
    public:
        runtime_error(std::string message) : std::runtime_error(message) {}
    };

    class type_error : public runtime_error
    {
    public:
        type_error(int index, int actual_type, int expected_type) : runtime_error("Type mismatch at index " + std::to_string(index) + ": expected " + std::to_string(expected_type) + ", got " + std::to_string(actual_type))
        {
        }
    };

    class invalid_argument : public runtime_error
    {
    public:
        invalid_argument(std::string argument, std::string why) : runtime_error("Invalid argument '" + argument + "': " + why)
        {
        }
    };
}

#endif