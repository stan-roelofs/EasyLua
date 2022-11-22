#ifndef __EASYLUA_EXCEPTION_H
#define __EASYLUA_EXCEPTION_H

#include <exception>
#include <string>

namespace easylua
{
    class Exception : public std::exception
    {
    protected:
        Exception() {}
    };

    class OutOfMemoryException : public Exception
    {
    public:
        OutOfMemoryException(std::string msg) : Exception() {} // TODO
    };

    class TypeException : public Exception
    {
    public:
        TypeException(int index, int actual_type, int expected_type) : index_(index), actual_type_(actual_type), expected_type_(expected_type) {}
        const char *what() const noexcept override
        {
            const std::string msg = "Type mismatch at index " + std::to_string(index_) + ": expected " + std::to_string(expected_type_) + ", got " + std::to_string(actual_type_);
            return msg.c_str();
        }

    private:
        int index_;
        int actual_type_;
        int expected_type_;
    };

    class InvalidArgumentException : public Exception
    {
    public:
        InvalidArgumentException(std::string_view argument, std::string_view why) : argument_(argument), why_(why) {}
        const char *what() const noexcept override
        {
            const std::string msg = "Invalid argument '" + std::string(argument_) + "': " + std::string(why_);
            return msg.c_str();
        }

    private:
        std::string_view argument_;
        std::string_view why_;
    };

    class CallException : public Exception
    {
    public:
        CallException(std::string msg) : Exception() {} // TODO
    };
}

#endif