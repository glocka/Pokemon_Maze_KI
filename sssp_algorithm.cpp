//------------------------------------------------------------------------------
// sssp_algorithm.cpp
//
// Group: Group 13734, study assistant Christoph Maurer
//
// Authors: Gerald Birngruber (1530438)
// Paul Plessing (1530486)
// Richard Sadek (1531352)
//------------------------------------------------------------------------------
//

#include "sssp_algorithm.h"
#include "path.h"
#include <iostream>
#include <climits>

SsspAlgorithm::SsspAlgorithm(int node_count)
{
  size_ = node_count;
  graph_ = new vector<Edge>[node_count];
  distances_.resize(node_count);
  predecessor_.resize(node_count);
}


SsspAlgorithm::~SsspAlgorithm()
{
  delete[] graph_;
}

vector<int> SsspAlgorithm::getDistances() const
{
  return distances_;
}

void SsspAlgorithm::addEdge(const int from, const int to, const int weight)
{
  graph_[from].push_back({ to, weight });
}

int SsspAlgorithm::getSource() const
{
  return source_;
}

int SsspAlgorithm::getDestination() const
{
  return destination_;
}

void SsspAlgorithm::setSource(const int source)
{
  source_ = source;
}

void SsspAlgorithm::setDestination(const int destination)
{
  destination_ = destination;
}
