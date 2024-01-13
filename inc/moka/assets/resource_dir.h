#pragma once
#include <string>
#include <string.h>

namespace moka::assets
{
  std::string relativePath(const std::string& path);
  
  std::string& relativePath(std::string& path);
}

