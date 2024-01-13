#pragma once
#include <string>
#include <algorithm>

namespace moka::helpers
{

char* getCmdOption(char ** begin, char ** end, const std::string & option);

bool cmdOptionExists(char** begin, char** end, const std::string& option);

}
