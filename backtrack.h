//------------------------------------------------------------------------------
// backtrack.h
//
// Group: Group 13734, study assistant Christoph Maurer
//
// Authors: Gerald Birngruber (1530438)
// Paul Plessing (1530486)
// Richard Sadek (1531352)
//------------------------------------------------------------------------------
//

#ifndef BACKTRACK_H
#define BACKTRACK_H

#include "sssp_algorithm.h"
#include "path.h"
#include <stack>

using std::stack;

class Backtrack : public SsspAlgorithm
{
private:
  int smallest_;
  int current_weight_;
  bool* visited_;
  vector<Edge> shortest_path_;
  vector<Edge> current_path_;

  void calculate1(Edge edge);

public:
  Backtrack(int node_count);
  ~Backtrack();
  virtual void calculate();
  virtual Path getPath();
};

#endif // BACKTRACK_H
