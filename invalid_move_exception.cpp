//------------------------------------------------------------------------------
// invalid_move_exception.cpp
//
// Group: Group 13734, study assistant Christoph Maurer
//
// Authors: Gerald Birngruber (1530438)
// Paul Plessing (1530486)
// Richard Sadek (1531352)
//------------------------------------------------------------------------------
//

#include "invalid_move_exception.h"
#include "error.h"

//------------------------------------------------------------------------------
InvalidMoveException::InvalidMoveException()
{
}

//------------------------------------------------------------------------------
const char* InvalidMoveException::what() const noexcept
{
  return Error::INVALID_MOVE;
}