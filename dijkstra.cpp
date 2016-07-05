//------------------------------------------------------------------------------
// dijkstra.cpp
//
// Group: Group 13734, study assistant Christoph Maurer
//
// Authors: Gerald Birngruber (1530438)
// Paul Plessing (1530486)
// Richard Sadek (1531352)
//------------------------------------------------------------------------------
//

#include "dijkstra.h"
#include <queue>
#include <climits>
#include <iostream>

using std::priority_queue;

Dijkstra::Dijkstra(int node_count) : SsspAlgorithm(node_count)
{
}

Dijkstra::~Dijkstra()
{
}

void Dijkstra::calculate()
{
  priority_queue<Edge> edges;
  distances_.assign(size_, INT_MAX);
  distances_[source_] = 0;
  predecessor_[source_] = -1;

  edges.push({ 0, source_ });

  while (!edges.empty())
  {
    int u = edges.top().second;
    edges.pop();

    for (Edge &current : graph_[u])
    {
      int to = current.first;
      int weight = current.second;
      if (distances_[to] > distances_[u] + weight)
      {
        distances_[to] = distances_[u] + weight;
        predecessor_[to] = u;
        edges.push({ distances_[to], to });
      }
    }
  }
}

Path Dijkstra::getPath()
{
  Path path(distances_[destination_]);

  if (distances_[destination_] == INT_MAX)
  {
    return path;
  }


  path_.clear();
  int u = destination_;
  path_.push_back(u);
  while (predecessor_[u] != -1)
  {
    u = predecessor_[u];
    path_.push_back(u);
  }

  path.setPath(path_);
  return path;
}

int Dijkstra::getShortestPathLength() const
{
  return distances_[destination_];
}
