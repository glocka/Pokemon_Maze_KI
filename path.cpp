//------------------------------------------------------------------------------
// path.cpp
//
// Group: Group 13734, study assistant Christoph Maurer
//
// Authors: Gerald Birngruber (1530438)
// Paul Plessing (1530486)
// Richard Sadek (1531352)
//------------------------------------------------------------------------------
//

#include "path.h"

Path::Path(int length)
{
  length_ = length;
}

Path::Path(vector<int>& path, int length)
{
  path_ = path;
  length_ = length;
}

int Path::getLength() const
{
  return length_;
}

vector<int>& Path::getPath()
{
  return path_;
}

void Path::setLength(int length)
{
  length_ = length;
}

void Path::setPath(vector<int>& path)
{
  path_ = path;
}
