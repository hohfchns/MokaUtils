#pragma once
#include <string>

#define MOKA_DATA_DIR_NAME ".moka"
#define MOKA_EXE_PATH_MAX_LENGTH 255
#define MOKA_PROJECT_DB_NAME "projects_index.csv"

namespace moka::project
{
  struct Global {
    static std::string currentProjectPath;
    static std::string pathToExe;
    static std::string pathToExeDir;
  };

  std::string osDataDir();
  std::string dataDir();
  const std::string& exePath();
  const std::string& exeDir();

  std::string getProjectIndex(const std::string& projName);
  bool createProject(const std::string& projName, const std::string& projPath);

  int loadProjectFromCLI(int argc, char* argv[]);

  bool addProject(const std::string& projectName, const std::string& projectPath);
}



