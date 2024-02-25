/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include "glob.h"

#include "globFileSystemTest.h"


int main(int argc, char** argv)
{
    if(argc != 2)
    {
        printf("glob search");
        return 0;
    }

    // look for root direcotry without star

    std::vector<std::filesystem::path> paths;

    TestFileSystem(argv[1], paths);
    
    for(auto currentPath : paths)
    {
        std::cout << currentPath.filename() << " | " <<  currentPath << std::endl;
    }

    return 0;
}

