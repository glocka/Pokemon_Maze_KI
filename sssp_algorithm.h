#ifndef SSSP_H
#define SSSP_H


#include <vector>
#include <utility>
#include "path.h"


using std::vector;
using std::pair;

typedef pair<int,int> Edge;

class SsspAlgorithm
{
protected:
  vector<Edge>* graph_;
  vector<int> distances_;
  vector<int> predecessor_;
  vector<int> path_;

  int size_;
  int source_;
  int destination_;

public:
  SsspAlgorithm(int node_count);
  virtual ~SsspAlgorithm();

  virtual void calculate() = 0;
  virtual Path getPath() = 0;

  int getSource() const;
  int getDestination() const;
  vector<int> getDistances() const;

  void setSource(const int source);
  void setDestination(const int destination);
  void addEdge(const int from, const int to, const int weight);
};

#endif // SSSP_H
