#pragma once
#include <iostream>
#include <string>

namespace kern
{
    enum class DebugLevel
    {
        None,
        Warning,
        Error,
        Everything
    };
}

inline kern::DebugLevel debug;

inline void cast(std::string msg, kern::DebugLevel level)
{
    if (debug != kern::DebugLevel::None)
    {
        if ((debug == kern::DebugLevel::Warning && level == kern::DebugLevel::Warning) || (debug == kern::DebugLevel::Error && level == kern::DebugLevel::Warning) || (debug == kern::DebugLevel::Everything && level == kern::DebugLevel::Warning))
        {
            std::cout << "[KERN] [WARNING] " << msg << std::endl;
        }
        else if ((debug == kern::DebugLevel::Error && level == kern::DebugLevel::Error) || (debug == kern::DebugLevel::Everything && level == kern::DebugLevel::Error))
        {
            std::cout << "[KERN] [ERROR] " << msg << std::endl;
        }
        else if ((debug == kern::DebugLevel::Everything && level == kern::DebugLevel::Everything))
        {
            std::cout << "[KERN] " << msg << std::endl;
        }
    }
}

inline void cast(std::string msg)
{
    if (debug != kern::DebugLevel::None)
    {
        if (debug == kern::DebugLevel::Everything)
        {
            std::cout << "[KERN] " << msg << std::endl;
        }
    }
}