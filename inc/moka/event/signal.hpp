#pragma once
#include "moka/event/typedefs.h"
#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>
#include "moka/event/macros.h"

namespace moka::event
{

template<class... P>
class Signal
{
private:
  std::vector<std::function<void(P...)>> _targets;

public:
  void Connect(std::function<void(P...)> targetFunc)
  {
    _targets.push_back(targetFunc);
  }

  void Emit(const P&... params)
  {
    auto t = _targets.begin();
    while (t != _targets.end())
    {
      std::function<void(P...)>& cb = *t;
      
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

  void Disconnect(std::string id)
  {
    _targets.erase(id);
  }
};

}
