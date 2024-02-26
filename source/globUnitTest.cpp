#include "globUnitTest.h"

#include "glob.h"
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

struct StringToTest
{
     bool test;
     std::string stringToTest;
};

std::vector<StringToTest> filePathsToCheck = 
{
  { true, "c:/users/daniel/game/file.txt"                },   // 0                 
  { true, "c:/users/daniel/game/check/other/file.txt"    },   // 1                             
  { true, "c:/users/ben/game/check/other/file.txt"       },   // 2                         
  { true, "c:/users/ben/test/game/check/other/file.txt"  },   // 3                             
  { true, "c:/users/ben/test/game/check/other/file.mp4"  },   // 4                             
  { true, "c:/users/test.txt"                            },   // 5 
  { true, "c:/users/other/test.txt"                      },   // 6     
  { true, "d:/users/ben/documents/check.mp4"             },   // 7
};

struct GlobRequest
{
    std::string request;
    std::vector<int> expectedSolutions;
};

std::vector<GlobRequest> requests
{
    { "c:/users/*/game/*", { 0 } },
    { "c:/users/**.txt", { 0, 1, 2, 3, 5, 6 } },
    { "c:/users/*.txt", { 5 } },
    { "c:/**", { 0, 1, 2, 3, 4, 5, 6 } },
    { "?:/us**ent**4*", { 7 } }
};


bool RunGlobTest(const GlobRequest& request)
{
    bool success = true;
    for(int i= 0; i < filePathsToCheck.size(); ++i)
    {
        if(!filePathsToCheck[i].test) continue;
        
        bool succeeded = Glob::QueryGlob(request.request, filePathsToCheck[i].stringToTest);
        
        bool expectedToSucceed = request.expectedSolutions.end() != std::find(request.expectedSolutions.begin(), request.expectedSolutions.end(), i);
        
        bool matchSuccess = succeeded == expectedToSucceed;
        
        if(!matchSuccess)
        {
            std::cout << "Should be ";
            
            if(expectedToSucceed)
            {
                std::cout << "success, but we got failure | ";
            }
            else
            {
                std::cout << "failure, but we got success | ";
            }
            
            std::cout << "\"" << request.request << "\" << " << filePathsToCheck[i].stringToTest << std::endl;
        
            success = false;
        }
    }
    
    return success;
}

void RunAllGlobTests()
{
    bool failure = false;
    for(auto& globRequest : requests)
    {
        failure |= !RunGlobTest(globRequest);
    }
    
    if(failure)
    {
        std::cout << "Failed!" << std::endl;
    }
    else
    {
        std::cout << "Succeeded!" << std::endl;
    }
}