#pragma once
#include "moka/event/typedefs.h"
#include "moka/event/macros.h"

#include <functional>
#include <map>
#include <optional>
#include <random>
#include <algorithm>
#include <stdexcept>

static const size_t ID_LENGTH = 8;

static std::string gen_random_string(size_t length) {
    static const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, chars.size() - 1);
    
    std::string result(length, '\0');
    for(char& c : result) c = chars[dis(gen)];
    return result;
}

namespace moka::event
{

template<class... P>
class Signal
{
private:
  std::map<std::string, std::function<void(P...)>> _targets;

public:
  void Connect(std::function<void(P...)> targetFunc, const std::optional<std::string>& id = std::nullopt)
  {
    std::string finalId;
    if (id.has_value())
    {
      finalId = id.value();
    }
    else
    {
      finalId = gen_random_string(ID_LENGTH);
      while (_targets.find(finalId) != _targets.end())
      {
        finalId = gen_random_string(ID_LENGTH);
      }
    }

    if (_targets.find(finalId) != _targets.end())
    {
      throw std::runtime_error(std::string("Cannot connect with duplicate id `") + finalId + "`");
    }

    _targets[finalId] = targetFunc;
  }

  void Emit(const P&... params)
  {
    auto t = _targets.begin();
    while (t != _targets.end())
    {
      std::function<void(P...)>& cb = t->second;
      
      try
      {
        cb(params...);
      }
      catch (std::bad_function_call &exception)
      {
        t = _targets.erase(t);
        continue;
      }

      ++t;
    }
  }

  void Disconnect(const std::string& id)
  {
    auto found = _targets.find(id);
    if (found == _targets.end())
    {
      return;
    }

    _targets.erase(found);
  }
};

}
