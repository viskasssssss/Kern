#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

namespace kern {
    inline std::string readFile(const std::string& filepath)
    {
        std::string content;
        std::ifstream fileStream(filepath, std::ios::in);

        if (!fileStream.is_open()) {
            std::cerr << "Could not open file: " << filepath << std::endl;
            return "";
        }

        std::stringstream buffer;
        buffer << fileStream.rdbuf();
        content = buffer.str();
        fileStream.close();
        return content;
    }
}
