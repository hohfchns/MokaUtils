#pragma once
#include <string>

namespace moka::assets
{
  std::string readFile(const std::string& filePath, bool absolute=false, std::string* outFinalPath=nullptr);
}

