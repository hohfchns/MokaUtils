#pragma once

namespace moka::misc
{

template <class T>
class Singleton
{
private:
 Singleton() { }

public:
  static T& Ref() {
      static T base;
      return base; 
    }

  Singleton(const Singleton<T>& other) = delete;
  Singleton(Singleton<T>& other) = delete;
};

}
