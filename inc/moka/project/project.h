#pragma once
#include <string>

#define DATA_DIR_NAME ".moka"
#define PROJECT_DB_NAME "projects_index.csv"

namespace moka::project
{
  struct Global {
    static std::string currentProjectPath;
  };

  std::string osDataDir();
  std::string dataDir();

  std::string getProjectIndex(const std::string& projName);
  bool createProject(const std::string& projName, const std::string& projPath);

  int loadProjectFromCLI(int argc, char* argv[]);

  bool addProject(const std::string& projectName, const std::string& projectPath);
}



