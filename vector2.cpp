//------------------------------------------------------------------------------
// vector2.cpp
//
// Group: Group 13734, study assistant Christoph Maurer
//
// Authors: Gerald Birngruber (1530438)
// Paul Plessing (1530486)
// Richard Sadek (1531352)
//------------------------------------------------------------------------------
//

#include "vector2.h"
#include "game.h"

//------------------------------------------------------------------------------
Vector2::Vector2() : x_(0), y_(0)
{
}

//------------------------------------------------------------------------------
Vector2::Vector2(const int index)
{
  x_ = index % Game::Size.getX();
  y_ = index / Game::Size.getX();
}

//------------------------------------------------------------------------------
Vector2::Vector2(const int x, const int y) : x_(x), y_(y)
{
}

//------------------------------------------------------------------------------
bool Vector2::operator==(const Vector2& vector) const
{
  if (vector.getX() == x_ && vector.getY() == y_)
  {
    return true;
  }
  else
  {
    return false;
  }
}

//------------------------------------------------------------------------------
bool Vector2::operator!=(const Vector2& vector) const
{
  return !(operator==(vector));
}

Vector2 Vector2::operator+(const Vector2& other) const
{
  return Vector2(x_ + other.getX(), y_ + other.getY());
}

//------------------------------------------------------------------------------
void Vector2::set(const int x, const int y)
{
  x_ = x;
  y_ = y;
}

//------------------------------------------------------------------------------
void Vector2::moveby(const int dx, const int dy)
{
  x_ += dx;
  y_ += dy;
}

//------------------------------------------------------------------------------
int Vector2::getIndex() const
{
  return y_ * Game::Size.getX() + x_;
}

//------------------------------------------------------------------------------
int Vector2::getX() const
{
  return x_;
}

//------------------------------------------------------------------------------
int Vector2::getY() const
{
  return y_;
}
