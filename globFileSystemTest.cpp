#include "globFileSystemTest.h"
#include "glob.h"

void TestFileSystem(std::string_view glob, std::vector<std::filesystem::path>& outPaths)
{
    std::string rootPath = std::string(Glob::GetRootPathFromGlob(glob));
    std::cout << glob << " >> " << rootPath << std::endl;

    for(auto currentDirectory : std::filesystem::recursive_directory_iterator(rootPath))
    {
        std::string pathFromDirectory = currentDirectory.path();

        if(Glob::QueryGlob(glob, pathFromDirectory))
        {
            outPaths.push_back(currentDirectory);
        }
    }
}