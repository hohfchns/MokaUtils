#include "moka/datatypes/observable.hpp"
#include "moka/macros/assert.h"
#include <gtest/gtest.h>

using namespace moka::utils::datatypes;

static int s_num_called = 0;
static void callback(const std::string& message)
{
  s_num_called++;
  std::cout << message << std::endl;
}

class StaticClass {

public:
  static void Callback(const std::string& message)
  {
    num_called++;
    std::cout << message << std::endl;
  }

  static int num_called;
};

int StaticClass::num_called = 0;

class LocalClass {

public:
  void Callback(const std::string& message)
  {
    num_called++;
    std::cout << message << std::endl;
  }

  int num_called = 0;
};

struct Vec2 {
  Vec2 operator+(const Vec2& other) const
  {
    return Vec2{
      x + other.x,
      y + other.y
    };
  }

  std::string Str() const
  {
    return std::string("{ ") + std::to_string(x) + ", " + std::to_string(y) + " }";
  }

  int x, y;
};

struct Color {
  int r, g, b;

  std::string Str() const
  {
    return std::string("{ ") + std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + " }";
  }

  Color operator+(const Color& other) const
  {
    return Color { r + other.r, g + other.g, b + other.b };
  }

  Color operator-(const Color& other) const
  {
    return Color { r - other.r, g - other.g, b - other.b };
  }
};

template<typename T>
struct Wrapped {
public:
  T object;
};

class Rect {

public:
  Rect() :
  pos(
    [this](const Vec2& value) { this->SetPos(value); },
    [this]() -> const Vec2& { return this->GetPos(); }
  ),
  color(
    [this](const Color& value) { this->SetColor(value); },
    [this]() -> const Color& { return this->GetColor(); }
  )
  {
    *pos = {0, 0};
    *color = {0, 0, 0};

    child_color.object = {0, 0, 0};
  }

  LocalClass local_class;

  Wrapped<Color> child_color;

  Observable<Vec2, Rect> pos;
  Observable<Color, Rect> color;

  Observable<float, Rect> size_x;
  Observable<float, Rect> size_y;

  void SetPos(const Vec2& value)
  {
    *pos = value;
    callback("Rect | Global function callback");
    StaticClass::Callback("Rect | Static function callback");
  }

  const Vec2& GetPos()
  {
    return *pos;
  }

  void SetColor(const Color& value)
  {
    local_class.Callback("Rect | Local class callback");
    child_color.object = value;
    *color = child_color.object;
  }

  const Color& GetColor()
  {
    return child_color.object;
  }

};

TEST(MokaUtils, Observable) {
  Rect rect;

  rect.pos += {5, 1};
  rect.color = Color{0, 0, 0};
  rect.child_color.object = Color{255, 50, 20};

  MOKA_ASSERT(s_num_called == 1, std::string("Global callback called wrong number of times ") + "(" + std::to_string(s_num_called) + ")");
  MOKA_ASSERT(StaticClass::num_called == 1, std::string("Static Class callback called wrong number of times ") + "(" + std::to_string(s_num_called) + ")");
  MOKA_ASSERT(rect.local_class.num_called == 1, std::string("Local callback called wrong number of times ") + "(" + std::to_string(s_num_called) + ")");

  std::cout << "Position is: " << rect.pos->Str() << std::endl;
  std::cout << "Color is: " << rect.color->Str() << std::endl;
  std::cout << "Child Color is: " << rect.child_color.object.Str() << std::endl;

  MOKA_ASSERT(rect.pos->Str() == "{ 5, 1 }", "Position not set properly (got " + rect.pos->Str() + ")");
  MOKA_ASSERT(rect.color->Str() == rect.child_color.object.Str(), "Color not set properly (got " + rect.color->Str() + ")");

  rect.pos += Vec2{-1, 1};
  rect.pos += Vec2{10, -50};

  std::cout << "Position is: " << rect.pos->Str() << std::endl;
  MOKA_ASSERT(rect.pos->Str() == "{ 14, -48 }", "Position not set properly (got " + rect.pos->Str() + ")");
  MOKA_ASSERT(s_num_called == 3, std::string("Global callback called wrong number of times ") + "(" + std::to_string(s_num_called) + ")");

  rect.pos = Vec2{INT_MAX, INT_MAX};
  rect.pos = Vec2{INT_MIN, INT_MIN};

  MOKA_ASSERT(s_num_called == 5, std::string("Global callback called wrong number of times ") + "(" + std::to_string(s_num_called) + ")");
  MOKA_ASSERT(StaticClass::num_called == 5, std::string("Global callback called wrong number of times ") + "(" + std::to_string(s_num_called) + ")");

  rect.size_x = 50.f;
  rect.size_y = 20.f;

  MOKA_ASSERT(rect.size_x == 50.f, "size_x not set properly (got " + std::to_string(rect.size_x) + ")");
  MOKA_ASSERT(rect.size_y == 20.f, "size_x not set properly (got " + std::to_string(rect.size_y) + ")");

  rect.color -= {3, 3, 0};

  MOKA_ASSERT(rect.color->Str() == "{ 252, 47, 20 }", "Color not set properly (got " + rect.color->Str() + ")");

  rect.color += {3, 3, 1};

  MOKA_ASSERT(rect.color->Str() == "{ 255, 50, 21 }", "Color not set properly (got " + rect.color->Str() + ")");
}
