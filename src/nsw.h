#ifndef NSW_H
#define NSW_H

#include "commons.h"

typedef vector<vector<int>> Graph;

class NSW {
public:
  int M = 32;              // number of nearest neighbour to connect to
  int efConstruction = 32; // memory buffer during construction of trees.
  int efInference = 20;    // mermory buffer during nn search
  int K = 10;              // number of nearest neighbour to return
  int L = 2; // number of random runs to perform during nearest neighbour scan

  NSW();
  vvpif Search(const vvf &queries);
  vpif Search(const vf &query, int K, int ef, int *dots);
  void BuildIndex(const vvf &documents);

  Graph graph; // adjency list

private:
  vvf documents;
  vector<int> elementOrder; // order of element

  void AddToGraph(Graph &graph, const vf &element);
};

// vvpif NSWSearch(vvf queries, vvf documents);

#endif