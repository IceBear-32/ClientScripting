#pragma once
#include <sstream>
#include <fstream>

auto getFileString = [](std::string file_path) {
    std::ifstream file(file_path);
    std::stringstream filestr;
    if (file.is_open()) {
        filestr << file.rdbuf();
        file.close();
    }
    return filestr.str();
};