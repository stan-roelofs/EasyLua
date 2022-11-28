#ifndef __EASYLUA_EXCEPTION_H
#define __EASYLUA_EXCEPTION_H

#include <stdexcept>
#include <string>

namespace easylua
{
    class Exception : public std::runtime_error
    {
    protected:
        Exception(std::string message) : std::runtime_error(message) {}
    };

    class MemoryAllocationException : public Exception
    {
    public:
        MemoryAllocationException() : Exception("Memory allocation failed") {}
    };

    class TypeException : public Exception
    {
    public:
        TypeException(int index, int actual_type, int expected_type) : Exception("Type mismatch at index " + std::to_string(index) + ": expected " + std::to_string(expected_type) + ", got " + std::to_string(actual_type))
        {
        }
    };

    class InvalidArgumentException : public Exception
    {
    public:
        InvalidArgumentException(std::string argument, std::string why) : Exception("Invalid argument '" + argument + "': " + why)
        {
        }
    };

    class CallException : public Exception
    {
    public:
        CallException(std::string reason) : Exception("Call failed: " + reason) {}
    };
}

#endif