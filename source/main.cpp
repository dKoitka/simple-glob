/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include "glob.h"

#include <string.h>
#include "globUnitTest.h"
#include "globFileSystemTest.h"

int mainUnitTest(int argc, char** argv)
{
    RunAllGlobTests();

    return 0;
}

int mainFileSystem(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("glob search");
        return 0;
    }

    std::vector<std::filesystem::path> paths;

    TestFileSystem(argv[1], paths);
    
    for(auto currentPath : paths)
    {
        std::cout << currentPath << std::endl;
    }

    return 0;
}

int main(int argc, char** argv)
{
    if(argc == 2)
    {
        if(strcmp(argv[1], "--unit-test") == 0)
        {
            return mainUnitTest(argc, argv);
        }
        else
        {
            return mainFileSystem(argc, argv);
        }
    }
}

