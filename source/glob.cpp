#include "glob.h"

#include <iostream>
#include <string>
#include <vector>
#include <string_view>

namespace Glob
{
    enum class SearchState
    {
        invalid = -1,
        normal,
        asterisk,
        doubleAsterisk
    };

    SearchState GetNewState(const SearchState state, const std::string_view glob, const int globPointer)
    {
        const char currentGlobCharacter = glob[globPointer];

        // Move into an appropriate state if we're at a specific character
        switch (state)
        {
        case SearchState::normal:
        {
            if (currentGlobCharacter == '*')
            {
                return GetNewState(SearchState::asterisk, glob, globPointer);
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
                    return GetNewState(SearchState::doubleAsterisk, glob, globPointer);
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
                    return SearchState::invalid;
                }
            }
        }
        break;
        case SearchState::invalid:
        {
            return SearchState::invalid;
        }
        }

        return state;
    }

    bool GlobStep(std::string_view glob, std::string_view stringToTest, SearchState state = SearchState::normal, int globPointer = 0, int stringPointer = 0)
    {
        while (globPointer <= glob.size() && stringPointer <= stringToTest.size())
        {
            state = GetNewState(state, glob, globPointer);

            // check final character
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

            const char currentGlobCharacter = glob[globPointer];
            const char currentStringCharacter = stringToTest[stringPointer];

            switch (state)
            {
            case SearchState::normal:
            {
                if (currentGlobCharacter == '?' && currentStringCharacter != '/')
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

                    // std::cout << glob << " | " << stringToTest << " | " << currentGlobCharacter << " | " << nextGlobCharacter << std::endl;
                    //  check next character if it matches the string

                    // if it matches, start a normal check
                    if (currentStringCharacter == '/')
                    {
                        // next string character is a slash, so escape
                        return GlobStep(glob, stringToTest, SearchState::normal, globPointer + 1, stringPointer);
                    }
                    else if (currentStringCharacter == nextGlobCharacter || nextGlobCharacter == '?')
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
            case SearchState::invalid:
            {
                return false;
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
        int lastPathSeparator = 0;

        for (int characterIndex = 0; characterIndex < glob.size(); ++characterIndex)
        {
            const char currentGlobCharacter = glob[characterIndex];

            if (currentGlobCharacter == '/')
            {
                lastPathSeparator = characterIndex + 1;
            }

            if (currentGlobCharacter == '?' || currentGlobCharacter == '*')
            {
                return glob.substr(0, lastPathSeparator);
            }
        }

        return glob;
    }

}