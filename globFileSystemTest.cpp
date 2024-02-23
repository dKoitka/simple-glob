#include "globFileSystemTest.h"
#include "glob.h"

void TestFileSystem(std::string_view glob, std::vector<std::filesystem::path>& outPaths)
{
    std::string rootPath = std::string(Glob::GetRootPathFromGlob(glob));
    std::cout << glob << " >> " << rootPath << std::endl;

    std::filesystem::directory_options options = std::filesystem::directory_options::follow_directory_symlink | std::filesystem::directory_options::skip_permission_denied;
    for(auto currentDirectory : std::filesystem::recursive_directory_iterator(rootPath, options))
    {
        std::string pathFromDirectory = currentDirectory.path();

        if(Glob::QueryGlob(glob, pathFromDirectory))
        {
            outPaths.push_back(currentDirectory);
        }
    }
}