#pragma once
#include <filesystem>

auto getFilesFromPath = [](std::string path) {
    return std::filesystem::directory_iterator(path);
};