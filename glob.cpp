#include "glob.h"

#include <iostream>
#include <string>
#include <vector>
#include <string_view>

namespace Glob
{
    enum class SearchState
    {
        normal,
        asterisk,
        doubleAsterisk
    };

    void ThrowError()
    {
        std::cout << "Dan, what the fuck. No tripple asterisks, lol" << std::endl;
    }

    void Report()
    {
        std::cout << "test" << std::endl;
    }

    int branchCount = 0;
    int iterationCount = 0;

    void PrintCounts()
    {
        std::cout
            << "stats:" << std::endl
            << "\tBranchCount: " << branchCount << std::endl
            << "\tIterationCount: " << iterationCount << std::endl;
    }

    void doPrint(const std::string &stringToTest, int stringPointer, const std::string &glob, int globPointer, SearchState state)
    {
        int integerSize = 2;

        std::string stateType;

        switch (state)
        {
        case SearchState::normal:
            stateType = "normal";
            break;
        case SearchState::asterisk:
            stateType = "asterisk";
            break;
        case SearchState::doubleAsterisk:
            stateType = "doubleAsterisk";
            break;
        default:
            stateType = "????";
            break;
        }

        printf("%s %*d | %s %*d - state: %s\n", stringToTest.c_str(), integerSize, stringPointer, glob.c_str(), integerSize, globPointer, stateType.c_str());
        printf("%*s%*s\n", stringPointer + 1, "^", (int)((stringToTest.size() - stringPointer) + 4 + integerSize + globPointer), "^");
    }

#define PrintInfo() doPrint(std::string(stringToTest), stringPointer, std::string(glob), globPointer, state);

    // assume no ***
    bool GlobStep(std::string_view glob, std::string_view stringToTest, SearchState state = SearchState::normal, int globPointer = 0, int stringPointer = 0)
    {
        branchCount++;

        while (globPointer <= glob.size() && stringPointer <= stringToTest.size())
        {
            iterationCount++;

            const char currentGlobCharacter = glob[globPointer];
            const char currentStringCharacter = stringToTest[stringPointer];

            // Move into an appropriate state if we're at a specific character
            switch (state)
            {
            case SearchState::normal:
            {
                if (currentGlobCharacter == '*')
                {
                    return GlobStep(glob, stringToTest, SearchState::asterisk, globPointer, stringPointer);
                }
            }
            break;
            case SearchState::asterisk:
            {
                if (globPointer < (glob.size() - 1))
                {
                    const char nextGlobCharacter = glob[globPointer + 1];

                    if (nextGlobCharacter == '*')
                    {
                        return GlobStep(glob, stringToTest, SearchState::doubleAsterisk, globPointer, stringPointer);
                    }
                }
            }
            break;
            case SearchState::doubleAsterisk:
            {
                if (globPointer < (glob.size() - 2))
                {
                    const char nextGlobCharacter = glob[globPointer + 2];

                    if (nextGlobCharacter == '*')
                    {
                        ThrowError();

                        return false;
                    }
                }
            }
            break;
            }

            if (stringPointer == stringToTest.size())
            {
                // only successful if the current glob pointer is complete, or the last character is an asterisk
                if (globPointer == glob.size())
                {
                    return true;
                }
                else if (globPointer == glob.size() - 1 && state == SearchState::asterisk)
                {
                    return true;
                }
                else if (globPointer == glob.size() - 2 && state == SearchState::doubleAsterisk)
                {
                    return true;
                }

                // we still had part of the glob to check
                return false;
            }

            if (globPointer == glob.size())
            {
                // we shouldn't be at the end if not also at the end of the string, so we failed
                return false;
            }

            switch (state)
            {
            case SearchState::normal:
            {
                if (currentGlobCharacter == '*')
                {
                    return GlobStep(glob, stringToTest, SearchState::asterisk, globPointer, stringPointer);
                }

                if (currentGlobCharacter == '?')
                {
                    globPointer++;
                    stringPointer++;

                    continue;
                }

                // standard check against string
                if (currentGlobCharacter == currentStringCharacter)
                {
                    globPointer++;
                    stringPointer++;
                }
                else
                {
                    return false;
                }
            }
            break;
            case SearchState::asterisk:
            {
                if (globPointer < (glob.size() - 1))
                {
                    const char nextGlobCharacter = glob[globPointer + 1];

                    if (nextGlobCharacter == '*')
                    {
                        return GlobStep(glob, stringToTest, SearchState::doubleAsterisk, globPointer, stringPointer);
                    }
                    else
                    {
                        // std::cout << glob << " | " << stringToTest << " | " << currentGlobCharacter << " | " << nextGlobCharacter << std::endl;
                        //  check next character if it matches the string

                        // if it matches, start a normal check
                        if (currentStringCharacter == '/')
                        {
                            // next string character is a slash, so escape
                            return GlobStep(glob, stringToTest, SearchState::normal, globPointer + 1, stringPointer);
                        }
                        else if (currentStringCharacter == nextGlobCharacter)
                        {
                            if (GlobStep(glob, stringToTest, SearchState::normal, globPointer + 1, stringPointer))
                            {
                                return true;
                            }

                            stringPointer++;
                        }
                        else
                        {
                            // go next string character
                            stringPointer++;
                        }
                    }
                }
                else
                {
                    // final character, we cannot succeed with a slash, so check
                    if (currentStringCharacter == '/')
                    {
                        return false;
                    }
                    else
                    {
                        stringPointer++;
                    }
                }
            }
            break;
            case SearchState::doubleAsterisk:
            {
                if (globPointer < (glob.size() - 2))
                {
                    const char nextGlobCharacter = glob[globPointer + 2];

                    if (nextGlobCharacter == '*')
                    {
                        ThrowError();

                        return false;
                    }

                    // check next string character
                    //  check next character if it matches the string

                    // if it matches, start a normal check
                    if (currentStringCharacter == nextGlobCharacter)
                    {
                        if (GlobStep(glob, stringToTest, SearchState::normal, globPointer + 2, stringPointer))
                        {
                            return true;
                        }

                        stringPointer++;
                    }
                    else
                    {
                        // go next string character
                        stringPointer++;
                    }
                }
                else
                {
                    // if we're here, then we'll succeed everything
                    return true;
                }
            }
            break;
            }
        }

        std::cout << " Somehow we're here " << std::endl;

        return true;
    }

    bool QueryGlob(std::string_view glob, std::string_view stringToTest)
    {
        return GlobStep(glob, stringToTest);
    }

    std::string_view GetRootPathFromGlob(std::string_view glob)
    {
        // search for first wildcard
        // ignore square brackets for now
        for(int characterIndex = 0; characterIndex < glob.size(); ++characterIndex)
        {
            const char currentGlobCharacter = glob[characterIndex];

            if(currentGlobCharacter == '?' || currentGlobCharacter == '*')
            {
                return glob.substr(0, characterIndex);
            }
        }

        return glob;
    }

}