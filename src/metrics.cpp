#include "metrics.h"
#include <cstdio>
#include <unordered_set>
#include <iostream>

using std::unordered_set;

float Dot(const vf &a, const vf &b) {
  float s = 0;
  for (int i = 0; i < a.size(); i++) {
    s += a[i] * b[i];
  }
  return s;
}

void PrintMetrics(vvpif actual, vvpif output) {
  int nqueries = actual.size();
  int k = actual[0].size();

  float total = 0.;
  for (int i = 0; i < nqueries; i++) {
    int matches = 0;
    // index correct indexes
    unordered_set<int> gt_indexes;
    for (int j = 0; j < k; j++) {
      gt_indexes.insert(actual[i][j].first);
    }

    // checking if res indexes exists
    for (auto entry : output[i]) {
      if (gt_indexes.find(entry.first) != gt_indexes.end()) {
        matches++;
      }
    }
    // for (int j = 0; j < k; j++) {
    //   if (gt_indexes.find(output[i][j].first) != gt_indexes.end()) {
    //     matches++;
    //   }
    // }
    total += (1. * matches / k);
  }

  float precision = total / nqueries;
  printf("Recall: %0.2f\n", precision);
}