#include "bfnn.h"
#include "commons.h"
#include "metrics.h"
#include <numeric>

vvpif BFNN(vvf &queries, vvf &documents, int k) {
  vvpif result(0);
  result.reserve(queries.size());

  // range of integers from 0 to docsize-1
  vector<int> range(documents.size());
  iota(range.begin(), range.end(), 0);

  vf dotprods(documents.size(), 0);

  for (auto &query : queries) {

    // calculate dotprods
    for (int i = 0; i < documents.size(); i++) {
      dotprods[i] = Dot(query, documents[i]);
    }

    // sorting patially in decending order
    partial_sort(
        range.begin(), range.begin() + k, range.end(),
        [&dotprods](int i, int j) { return dotprods[i] > dotprods[j]; });

    // build results
    vpif res(k);
    for (int i = 0; i < k; i++) {
      res[i] = {range[i], dotprods[range[i]]};
    }

    result.push_back(res);
  }
  return result;
}
