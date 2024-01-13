#include "moka/assets/text_loader.h"
#include "moka/assets/resource_dir.h"
#include <sstream>
#include <string.h>
#include <fstream>

namespace moka::assets
{
  std::string readFile(const std::string& filePath, bool absolute, std::string* outFinalPath)
  {
    std::string newPath = relativePath(filePath);
    
    if (outFinalPath != nullptr)
    {
      *outFinalPath = newPath;
    }

    std::ifstream read(newPath);
    std::stringstream buffer;
    buffer << read.rdbuf();
    return buffer.str();
  }
}

