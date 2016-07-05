#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "vector2.h"
#include "sssp_algorithm.h"

using std::vector;

class Dijkstra : public SsspAlgorithm
{

public:
  Dijkstra(int node_count);
  virtual ~Dijkstra();

  virtual void calculate();
  virtual Path getPath();
  int getShortestPathLength() const;
};

#endif // DIJKSTRA_H
