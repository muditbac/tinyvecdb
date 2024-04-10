#include "nsw.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <queue>
#include <random>
#include <set>
#include <unordered_set>

#include "metrics.h"

using std::shuffle;

const int SEED = 2024;

int M = 32; // number of nearest neighbours to connect to
int ef_construction = 32;

// TODO used NSW's nn api to find nearest k elements
void NSW::AddToGraph(Graph &graph, const vf &element) {
  int nodes = graph.size();
  vector<int> currentNode(0);

  if (nodes % 1000 == 0) {
    std::cout << "\r" << nodes;
    std::flush(std::cout);
  }
  if (nodes == 0) {
    graph.push_back(currentNode);
    return;
  }

  auto res = Search(element, M, ef_construction, NULL);
  for (auto r : res) {
    int index = r.first;
    graph[index].push_back(nodes); // adding current element
    currentNode.push_back(index);
  }
  graph.push_back(currentNode);
}

void NSW::BuildGraph(const vvf &documents) {
  int ndocs = documents.size();

  for (auto &doc : documents) {
    AddToGraph(this->graph, doc);
  }
}

NSW::NSW(const vvf &documents) {
  this->documents.clear();
  this->documents = documents;
  // Enable shuffle if input is not random
  //   shuffle(this->documents.begin(), this->documents.end(),
  //           std::default_random_engine(SEED));
  BuildGraph(this->documents);
}

bool cmp(pif &a, pif &b) { return a.second > b.second; }
bool cmp2(pif &a, pif &b) { return a.second < b.second; }

class Comp2 {
public:
  bool operator()(pif &a, pif &b) { return a.second < b.second; }
};

void KeepTrackHighest(vector<pif> &minheap, pif &element, int size) {
  if (minheap.size() < size) {
    minheap.push_back(element);
    std::push_heap(minheap.begin(), minheap.end(), cmp);
    return;
  }
  if (element.second >= minheap[0].second) {
    std::pop_heap(minheap.begin(), minheap.end(), cmp);
    minheap.pop_back();
    minheap.push_back(element);
    std::push_heap(minheap.begin(), minheap.end(), cmp);
  }
}

vpif NSW::Search(const vf &query, int K = 10, int ef = 20, int *dots = NULL) {

  vector<pif> ans(0);
  ans.reserve(K);
  int nodes = this->graph.size();

  std::set<int> visited;
  visited.clear();

  if (dots != NULL)
    *dots = 1;

  //   repeating this to find K nearest neigbours
  for (int x = 0; x < 2; x++) {
    vector<pif> minheap(0);
    minheap.reserve(ef);

    // TODO Check if this needs to be a priority queue or a regular queue would
    // work. Also this currently keeps min element on top. Is this the right
    // beahviour?
    std::priority_queue<pif, vector<pif>, Comp2> q;
    // std::queue<pif> q;
    int currNode = rand() % nodes;
    auto currSim = Dot(query, this->documents[currNode]);

    q.push({currNode, currSim});

    while (q.size()) {
      //   pif node = q.front();
      pif node = q.top();
      q.pop();
      int currNode = node.first;
      float currSim = node.second;

      if (minheap.size() > 0 && currSim < minheap[0].second)
        break;
      if (visited.find(currNode) != visited.end()) // already visited node
        continue;
      visited.insert(node.first);

      KeepTrackHighest(minheap, node, ef);
      auto &neighbors = this->graph[currNode];

      for (auto idx : neighbors) {
        auto neighborSim = Dot(query, this->documents[idx]);
        if (dots != NULL)
          (*dots)++;

        // pre filtering, saves time
        // if (visited.count(idx) > 0) // already visited node
        //   continue;

        q.push({idx, neighborSim});
      }
    }

    // global optimum across all random searches
    for (auto each : minheap) {
      KeepTrackHighest(ans, each, K);
    }
  }
  // std::cout << "nhops " << visited.size() << "\n";
  // if (dots != NULL)
  //   std::cout << "dots " << *dots << "\n";

  return ans;
}

vvpif NSW::Search(const vvf &queries) {
  vvpif results(0);
  results.reserve(queries.size());
  int total = 0;
  for (auto &query : queries) {
    int dot = 0;
    results.push_back(this->Search(query, 10, 40, &dot));
    total += dot;
  }
  std::cout << "average dots: " << total * 1. / queries.size() << std::endl;
  return results;
}