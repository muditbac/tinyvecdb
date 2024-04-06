#include "nsw.h"
#include <algorithm>
#include <numeric>
#include <queue>
#include <random>
#include <unordered_set>

#include "metrics.h"

using std::shuffle;

const int SEED = 2024;

int eN = 4;

void NSW::AddToGraph(Graph &graph, const vf &element) {
  int nodes = graph.size();

  // finding dot products with all existing nodes
  vf dotprods(nodes, 0);
  for (int i = 0; i < nodes; i++) {
    dotprods[i] = Dot(element, this->documents[i]);
  }

  // finding nearest eN nodes
  vector<int> range(nodes, 0);
  iota(range.begin(), range.end(), 0);
  int e = std::min(eN, nodes);
  std::partial_sort(
      range.begin(), range.begin() + e, range.end(),
      [&dotprods](int i, int j) { return dotprods[i] > dotprods[j]; });

  // creating edges to nearest elements
  vector<int> currentNode(0);
  for (int i = 0; i < e; i++) {
    int index = range[i];
    graph[index].push_back(nodes); // adding current element
    currentNode.push_back(index);
  }

  // adding current elements
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
  //   shuffle(this->documents.begin(), this->documents.end(),
  //           std::default_random_engine(SEED));
  BuildGraph(this->documents);
}

class Compare {
public:
  bool operator()(pif &a, pif &b) { return a.second > b.second; }
};

bool cmp(pif &a, pif &b) { return a.second > b.second; }
bool cmp2(pif &a, pif &b) { return a.second < b.second; }

int K = 10;

void KeepTrackHighest(vector<pif> &minheap, pif element, int size = K) {
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

vpif NSW::Search(const vf &query) {
  // min heap
  //   std::priority_queue<pif, vector<pif>, Compare> minheap;
  vector<pif> ans(0);
  ans.reserve(K);
  int nodes = this->graph.size();

  std::unordered_set<int> visited;
  visited.clear();
  //   repeating this to find K nearest neigbours
  for (int x = 0; x < 5; x++) {
    vector<pif> minheap(0);
    minheap.reserve(K);

    // TODO Check if this needs to be a priority queue or a regular queue would
    // work. Also this currently keeps min element on top. Is this the right
    // beahviour?
    // std::priority_queue<pif, vector<pif>, std::function<bool(pif &, pif &)>>
    // q(
    //     cmp);
    std::queue<pif> q;
    int currNode = rand() % nodes;
    auto currSim = Dot(query, this->documents[currNode]);

    q.push({currNode, currSim});

    while (q.size()) {
      pif node = q.front();
      //   pif node = q.top();
      q.pop();
      int currNode = node.first;
      float currSim = node.second;
      if (visited.count(currNode) > 0) // already visited node
        continue;
      visited.insert(node.first);
      if (!minheap.empty() && currSim < minheap[0].second)
        continue;

      KeepTrackHighest(minheap, node, 10);
      auto neighbors = this->graph[currNode];

      for (auto idx : neighbors) {
        auto neighborSim = Dot(query, this->documents[idx]);

        // pre filtering, saves time
        if (!minheap.empty() && neighborSim < minheap[0].second)
          continue;

        q.push({idx, neighborSim});
      }
    }

    // global optimum across all random searches
    for (auto each : minheap) {
      KeepTrackHighest(ans, each);
    }
  }

  return ans;
}

vvpif NSW::Search(const vvf &queries) {
  vvpif results(0);
  for (auto &query : queries) {
    results.push_back(this->Search(query));
  }
  return results;
}