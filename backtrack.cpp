//------------------------------------------------------------------------------
// backtrack.cpp
//
// Group: Group 13734, study assistant Christoph Maurer
//
// Authors: Gerald Birngruber (1530438)
// Paul Plessing (1530486)
// Richard Sadek (1531352)
//------------------------------------------------------------------------------
//

#include "backtrack.h"
#include <climits>
#include <iostream>

Backtrack::Backtrack(int node_count) : SsspAlgorithm(node_count)
{
  visited_ = new bool[node_count]();
  smallest_ = INT_MAX;
  current_weight_ = 0;
}

Backtrack::~Backtrack()
{
  delete[] visited_;
}

void Backtrack::calculate1(Edge edge)
{
  int u = edge.first;
  int weight = edge.second;

  if (visited_[u] || current_weight_ + weight > smallest_)
    return;

  current_path_.push_back(edge);
  current_weight_ += weight;

  if (u == destination_)
  {
    if (current_weight_ < smallest_)
    {
      smallest_ = current_weight_;
      shortest_path_ = current_path_;
    }
  }

  visited_[u] = true;

  for (Edge v : graph_[u])
  {
    calculate1(v);
  }
  visited_[u] = false;
  current_path_.pop_back();
  current_weight_ -= weight;
}

void Backtrack::calculate()
{
  calculate1({source_, 0});
}

Path Backtrack::getPath()
{
  path_.clear();
  for (Edge node : shortest_path_)
  {
    path_.push_back(node.first);
  }

  Path path(path_.size());
  path.setPath(path_);

  return path;
}

/*void Backtrack::calculate()
{
  int count;

  stack<int> S;
  S.push(source_);
  while (!S.empty())
  {
    int v = S.top();
    S.pop();
    count--;
    current_path_.push(v);
    visited_[v] = true;

    for (auto w : graph_[v])
    {
      if (!visited_[w.first])
      {
        S.push(w.first);
        count++;
      }
    }
  }
}*/

/*bool Backtrack::calculate1(int node, int weight)
{

  current_path_.push(node);
  current_weight_ += weight;

  if (visited_[node])
    return false;

  visited_[node] = true;


  if (node == destination_)
    return true;

  for (auto &current : graph_[node])
  {
    int next = current.first;
    int weight = current.second;

    calculate1(next, weight);
    current_path_.pop();
    current_weight_ -= weight;
    visited_[next] = false;
  }
  return true;
}*/

