#ifndef NSW_H
#define NSW_H

#include "ds.h"

typedef vector<vector<int>> Graph;

class NSW {
public:
  NSW(const vvf &documents);
  vvpif Search(const vvf &queries);
  vpif Search(const vf &query, int K, int ef);

  Graph graph; // adjency list

private:
  vvf documents;
  vector<int> elementOrder; // order of element

  void AddToGraph(Graph &graph, const vf &element);
  void BuildGraph(const vvf &documents);
};

// vvpif NSWSearch(vvf queries, vvf documents);

#endif