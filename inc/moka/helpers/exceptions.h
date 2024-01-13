#pragma once
#include <stdexcept>

namespace moka
{

class bad_dealloc : public std::exception
{
  private:
    std::string message;
  public:
    bad_dealloc(const std::string& msg)
      : message(msg)
    {
    }

    virtual const char* what() const noexcept override
    {
      return message.c_str();
    }
};

class not_implemented : public std::logic_error
{
private:
public:
  not_implemented(const std::string& msg)
    : std::logic_error(msg)
  {
  }

  not_implemented()
    : std::logic_error("Function not implemented! Perhaps on purpose?")
  {
  }
};

}
