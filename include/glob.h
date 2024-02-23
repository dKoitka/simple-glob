#pragma once

#include <iostream>
#include <string_view>

namespace Glob
{
    // assume no ***
    bool QueryGlob(std::string_view glob, std::string_view stringToTest);

    std::string_view GetRootPathFromGlob(std::string_view glob);
}