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
inline std::string previousLog = "";

inline void cast(std::string msg, bool addNewLine)
{
    if (debug != kern::DebugLevel::None)
    {
        if (addNewLine)
        {
            std::cout << "[KERN] [INFO] " << msg << std::endl;
        }
        else
        {
            std::cout << "[KERN] [INFO] " << msg;
        }
    }
}

inline void cast(std::string msg, kern::DebugLevel level, bool newLineNeeded = true)
{
    if (!newLineNeeded)
    {
        if (debug != kern::DebugLevel::None)
        {
            std::cout << msg << std::endl;
            return;
        }
    }
    if (debug != kern::DebugLevel::None)
    {
        if ((debug == kern::DebugLevel::Warning && level == kern::DebugLevel::Warning) || (debug == kern::DebugLevel::Error && level == kern::DebugLevel::Warning) || (debug == kern::DebugLevel::Everything && level == kern::DebugLevel::Warning))
        {
            if (previousLog == msg)
            {
                return;
            }
            std::cout << "[KERN] [WARNING] " << msg << std::endl;
            previousLog = msg;
        }
        else if ((debug == kern::DebugLevel::Error && level == kern::DebugLevel::Error) || (debug == kern::DebugLevel::Everything && level == kern::DebugLevel::Error))
        {
            if (previousLog == msg)
            {
                return;
            }
            std::cout << "[KERN] [ERROR] " << msg << std::endl;
            previousLog = msg;
        }
        else if ((debug == kern::DebugLevel::Everything && level == kern::DebugLevel::Everything))
        {
            std::cout << "[KERN] [INFO] " << msg << std::endl;
        }
    }
}

inline void cast(std::string msg)
{
    if (debug != kern::DebugLevel::None)
    {
        if (debug == kern::DebugLevel::Everything)
        {
            std::cout << "[KERN] [INFO] " << msg << std::endl;
        }
    }
}