#include "moka/assets/resource_dir.h"
#include "moka/project/project.h"

namespace moka::assets
{
  std::string relativePath(const std::string& path)
  {
    return moka::project::Global::currentProjectPath + "/" + path;
  }

  std::string& relativePath(std::string& path)
  {
    path = (moka::project::Global::currentProjectPath + "/" + path);
    return path;
  }
}

