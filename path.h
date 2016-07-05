#ifndef PATH_H
#define PATH_H

#include <vector>

using std::vector;

class Path
{
private:
  vector<int> path_;
  int length_;
public:
  Path(int length);
  Path(vector<int>& path, int length);

  int getLength() const;
  vector<int>& getPath();
  void setLength(int length);
  void setPath(vector<int>& path);

};

#endif // PATH_H
