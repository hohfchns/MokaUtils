#pragma once
#include "moka/event/typedefs.h"
#include "moka/event/macros.h"
#include "moka/macros/assert.h"

#include <functional>
#include <map>
#include <optional>
#include <random>
#include <algorithm>
#include <stdexcept>

static const size_t ID_LENGTH = 8;

static inline std::string gen_random_string(size_t length) {
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
public:
  using Callback = void (P...);

  enum ConnectionFlags : size_t {
    CONNECT_ONE_SHOT = 1 << 0
  };

  struct Connection {
    Connection(std::function<void(P...)> targetFunc, void* object, const std::string& id, size_t flags)
    : object(object), targetFunc(targetFunc), flags(flags), id(id) {}

    void* object;
    std::function<void(P...)> targetFunc;
    size_t flags;
    std::string id;
  };

private:
  std::map<void*, std::map<std::string, Connection>> _targets;

public:
  void Connect(std::function<void(P...)> targetFunc, void* object = nullptr, const std::optional<std::string>& id = std::nullopt, size_t flags = 0)
  {
    std::string finalId;
    auto objectFound = _targets.find(object);
    if (id.has_value())
    {
      finalId = id.value();
    }
    else
    {
      finalId = gen_random_string(ID_LENGTH);
      if (objectFound != _targets.end())
      {
        while (_targets[object].find(finalId) != _targets[object].end())
        {
          finalId = gen_random_string(ID_LENGTH);
        }
      }
    }

    if (objectFound == _targets.end())
    {
      _targets[object] = {};
    }
    else
    {
      if (_targets[object].find(finalId) != _targets[object].end())
      {
        throw std::runtime_error(std::string("Cannot connect to object `b") + std::to_string((size_t)object) + "` with duplicate id `" + finalId + "`");
      }
    }

    _targets[object].emplace(finalId, Connection(targetFunc, object, finalId, flags));
  }

  void Emit(const P&... params)
  {
    auto itrObjectId = _targets.begin();
    while (itrObjectId != _targets.end())
    {
      auto& idConnection = itrObjectId->second;
      auto itrIdConnection = idConnection.begin();
      while (itrIdConnection != idConnection.end())
      {
        Connection& connection = itrIdConnection->second;
        
        try
        {
          connection.targetFunc(params...);
        }
        catch (std::bad_function_call &exception)
        {
          itrIdConnection = idConnection.erase(itrIdConnection);
          continue;
        }

        ++itrIdConnection;
      }

      ++itrObjectId;
    }
  }

  bool Disconnect(void* object, const std::string& id)
  {
    auto foundObject = _targets.find(object);
    if (foundObject == _targets.end())
    {
      return false;
    }

    auto found = _targets[object].find(id);
    if (found == _targets[object].end())
    {
      return false;
    }

    _targets[object].erase(found);
    if (_targets[object].size() == 0)
    {
      _targets.erase(object);
    }

    return true;
  }
};

}
