#pragma once

#include "moka/macros/assert.h"
#include <functional>

namespace moka::utils::datatypes
{

template<typename T, typename B>
class Observable {
public:

using ObservableSetterConst = std::function<void(const T&)>;
using ObservableGetterConst = std::function<const T&()>;
using ObservableGetterRef = std::function<T&()>;

  Observable(const T& from)
  {
    _instance = from;
  }

  Observable(Observable<T, B>&& from)
  {
    _instance = std::move(from._instance);
    setter_copy = std::move(from.setter_copy);
    getter_ref = std::move(from.getter_ref);
    getter_const = std::move(from.getter_const);
  }

  Observable()
  {
  }

  Observable(const ObservableSetterConst& setter_copy)
  {
    MOKA_ASSERT(setter_copy != nullptr, "Copy setter is null!");
    this->setter_copy = setter_copy;
  }


  Observable(const ObservableGetterConst& getter_const)
  {
    MOKA_ASSERT(getter_const != nullptr, "Const getter is null!");
    this->getter_const = getter_const;
  }

  Observable(const ObservableGetterRef& getter_ref)
  {
    MOKA_ASSERT(getter_ref != nullptr, "Ref getter is null!");
    this->getter_ref = getter_ref;
  }

  Observable(const ObservableGetterConst& getter_const, const ObservableGetterRef& getter_ref)
  {
    MOKA_ASSERT(getter_const != nullptr, "Const getter is null!");
    MOKA_ASSERT(getter_ref != nullptr, "Ref getter is null!");
    this->getter_const = getter_const;
    this->getter_ref = getter_ref;
  }

  Observable(const ObservableSetterConst& setter_copy, const ObservableGetterConst& getter_const)
  {
    MOKA_ASSERT(setter_copy != nullptr, "Copy setter is null!");
    MOKA_ASSERT(getter_const != nullptr, "Const getter is null!");
    this->setter_copy = setter_copy;
    this->getter_const = getter_const;
  }

  Observable(const ObservableSetterConst& setter_copy, const ObservableGetterConst& getter_const, const ObservableGetterRef& getter_ref)
  {
    MOKA_ASSERT(getter_const != nullptr, "Const getter is null!");
    MOKA_ASSERT(getter_ref != nullptr, "Ref getter is null!");
    this->setter_copy = setter_copy;
    this->getter_const = getter_const;
    this->getter_ref = getter_ref;
  }

  Observable<T, B>&  operator=(const Observable<T, B>& from)
  {
    _instance = from._instance;

    return *this;
  }

  Observable<T, B>&  operator=(Observable<T, B>&& from)
  {
    _instance = std::move(from._instance);
    setter_copy = std::move(from.setter_copy);
    getter_ref = std::move(from.getter_ref);
    getter_const = std::move(from.getter_const);

    return *this;
  }

  void _Set(const T& value)
  {
    _instance = value;
  }

  const T& _Get() const
  {
    return _instance;
  }

  T& _Get()
  {
    return _instance;
  }

  void Set(const T& value)
  {
    if (setter_copy == nullptr)
    {
      _Set(value);
      return;
    }

    setter_copy(value);
  }

  const T& Get() const
  {
    if (getter_const == nullptr)
    {
      return _Get();
    }

    return getter_const();
  }

  T& Get()
  {
    if (getter_ref == nullptr)
    {
      return _Get();
    }
    return getter_ref();
  }

  operator T& ()
  {
    return Get();
  }

  operator const T& () const
  {
    return Get();
  }

  void operator=(const T& value)
  {
    Set(value);
  }

  void operator-=(const T& value)
  {
    const T& current_val = getter_ref == nullptr ?
    static_cast<const Observable<T, B>*>(this)->Get()
    :
    static_cast<Observable<T, B>*>(this)->Get();

    Set(
      current_val - value
    );
  }

  void operator+=(const T& value)
  {
    const T& current_val = getter_ref == nullptr ?
    static_cast<const Observable<T, B>*>(this)->Get()
    :
    static_cast<Observable<T, B>*>(this)->Get();

    Set(
      current_val + value
    );
  }

  T operator+(const T& other)
  {
    return Get() + other;
  }

  T operator-(const T& other)
  {
    return Get() - other;
  }

  const T* operator->() const
  {
    return &Get();
  }

  const T& operator*() const
  {
    return _instance;
  }

  T& operator*()
  {
    return _instance;
  }

public:
  ObservableSetterConst setter_copy = nullptr;
  ObservableGetterConst getter_const = nullptr;
  ObservableGetterRef getter_ref = nullptr;

private:
  T _instance;
};


}

