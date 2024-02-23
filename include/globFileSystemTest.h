#pragma once
#include <string_view>
#include <vector>
#include <filesystem>

void TestFileSystem(std::string_view glob, std::vector<std::filesystem::path>& outPaths);