#include "moka/project/project.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include "moka/helpers/cli_parser.h"

using namespace std;
using namespace filesystem;
using namespace moka::helpers;

namespace moka::project
{

std::string Global::currentProjectPath = "";
std::string Global::pathToExe = "";
std::string Global::pathToExeDir = "";

string osDataDir()
{
#ifdef __WIN32__
  return getenv("APPDATA");
#endif
#ifdef __linux__
  return getenv("HOME");
#endif
}

string dataDir() 
{
  const string dir = path(osDataDir() + "/" + MOKA_DATA_DIR_NAME);
  if (!exists(dir))
  {
    create_directory(dir);
  }

  return dir;
}

const std::string& exePath()
{
  if (Global::pathToExe.empty())
  {
#ifdef __linux__
  Global::pathToExe = std::filesystem::canonical("/proc/self/exe");
#endif
#ifdef __WIN32__
  char path[EXE_PATH_MAX_LENGTH]
  GetModuleFileName(NULL, path, EXE_PATH_MAX_LENGTH)
  Global::pathToExe = path;
#endif
  }

  return Global::pathToExe;
}

const std::string& exeDir()
{
  if (Global::pathToExeDir.empty())
  {
    Global::pathToExeDir = std::filesystem::path(exePath()).parent_path();
  }

  return Global::pathToExeDir;
}

string getProjectIndex(const string& projName)
{
  const path projectIdxPath = dataDir() + "/" + MOKA_PROJECT_DB_NAME;

  ifstream readIndex(projectIdxPath);

  string line;
  while ( getline(readIndex, line) )
  {
    stringstream tmp(line);
    string first;
    string second;
    getline(tmp, first, ',');
    getline(tmp, second, ',');

    if ( first == projName )
    {
      return second;
    }
    else if (exists(projName) && second==projName)
    {
      return second;
    }
  }

  readIndex.close();
  return "";
};

bool createProject(const string& projName, const string& projPath)
{
  const path projectIdxPath = dataDir() + "/" + MOKA_PROJECT_DB_NAME;
  const string csvInsert = projName + "," + projPath;

  fstream indexFile(projectIdxPath, ios::in | ios::out | ios::app);

  string line;
  while ( getline(indexFile, line) )
  {
    stringstream tmp(line);
    string first;
    string second;
    getline(tmp, first, ',');
    getline(tmp, second, ',');
    
    if ( first == projName )
    {
      cout << "ERROR! Tried to create a project that already exists in index!" << endl;
      return false;
    }
  }

  if ( !exists(projPath) )
  {
    cout << "Creating directory '" << projPath << "'" << endl;
    create_directory(projPath);
  }

  if ( filesystem::is_empty(projPath) )
  {
    cout << "Copying engine resources to '" << projPath << "/bundled'" << endl;
    filesystem::copy(dataDir() + "/bundled", projPath, copy_options::recursive);
  }
  else 
  {
    cout << "ERROR! Cannot create project as it already exists on disk!" << endl;
    return false;
  }

  indexFile.clear();
  indexFile << csvInsert << "\n";
  indexFile.close();
  return true;
};

bool addProject(const string& projectName, const string& projectPath)
{
  string existing = getProjectIndex(projectPath);

  if (existing != "")
  {
    cout << "Tried to add project that already exists" << endl;
    return false;
  }

  const path projectIdxPath = dataDir() + "/" + MOKA_PROJECT_DB_NAME;
  ofstream indexFile(projectIdxPath, ios_base::app);

  indexFile.clear();
  indexFile << projectName << "," << projectPath << "\n";
  indexFile.close();

  return true;
}

int loadProjectFromCLI(int argc, char* argv[])
{
  if (argc <= 1)
  {
    cout << "Project " << argv[1] << " not found in index, consider adding or creating it" << endl;
    Global::currentProjectPath = dataDir() + "/bundled/";
    cout << "Reverting to default data directory '" << Global::currentProjectPath << "'" << endl;
    return 1;
  }

  const path projectIdxPath = dataDir() + "/" + MOKA_PROJECT_DB_NAME;

  if (!exists(projectIdxPath))
  {
    create_directories(projectIdxPath.parent_path());
    ofstream ofs(projectIdxPath);
    ofs << "\n"; 
    ofs.close();
  }

  // Default behaviour
  
  char* projName = argv[1];
  path dir;

  if (argc <= 2)
  {
    cout << "Attempting to load project / directory '" << projName << "'" << endl;
  }

  if ( exists(projName) )
  {
    dir = canonical(projName);
    cout << "Got argument for existing directory " << dir << endl;
    cout << "Checking if path is in index..." << endl;
    Global::currentProjectPath = getProjectIndex(dir);
  }

  if ( Global::currentProjectPath != "" )
  {
    cout << "Found path '" << Global::currentProjectPath << "' in index!" << endl;
    return 0;
  }
  else
  {
    cout << "Path '" << dir << "' not found in index, checking if it is project name..." << endl;
    Global::currentProjectPath = getProjectIndex(projName);
    if (Global::currentProjectPath != "")
    {
      cout << "Found project '" << Global::currentProjectPath << "' in index!" << endl;
      return 0;
    }
  }

  // Arguments
  
  char* addDir = getCmdOption(argv, argv + argc, "--add");
  if (addDir)
  {
    char* addName = getCmdOption(argv, argv + argc, addDir);
    if (!addName)
    {
      cout << "Please provide {dir}, {projname} after --add" << endl;
      return -1;
    }

    if(exists(addDir) && addProject(addName, canonical(addDir)))
    {
      cout << "Attempting to add directory '" << canonical(addDir) << "' with project name '" << addName << "'" << endl;
      Global::currentProjectPath = addDir;
      return 0;
    }
    else
    {
      cout << "Failed to add project" << endl;
      return -1;
    }
  }

  char* createDir = getCmdOption(argv, argv + argc, "--create");
  if (createDir )
  {
    if (!exists(createDir))
    {
      cout << "Directory '" << createDir << "' provided for --create does not exist" << endl;
    }

    string createDirAbs = canonical(createDir);
    char* createName = getCmdOption(argv, argv + argc, createDir);
    if (!createName)
    {
      cout << "Please provide {dir}, {projname} after --create" << endl;
      return -1;
    }

    cout << "Attempting to create directory '" << createDirAbs << "' with project name '" << createName << "'" << endl;
    if (createProject(createName, createDirAbs))
    {
      Global::currentProjectPath = createDirAbs;
      return 0;
    }
    else
    {
      cout << "Failed to create project" << endl;
      return -1;
    }
  }

  cout << "Project " << argv[1] << " not found in index, consider adding or creating it" << endl;
  Global::currentProjectPath = dataDir() + "/bundled/";
  cout << "Reverting to default data directory '" << Global::currentProjectPath << "'" << endl;

  return 1;
}

}
