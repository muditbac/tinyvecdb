#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <cmath>

#include "commons.h"
#include "metrics.h"
#include "nsw.h"
#include "bfnn.h"

using namespace std;
using namespace std::chrono;

int NQUERIES = 1000;
int NDOCUMENTS = 100000;
int NDIM = 128;

float Norm(const vf &vec) {
  float s = 0;
  for (const float &f : vec) {
    s += f * f;
  }
  return pow(s, .5);
}

void NormalizeVectors(vvf &matrix) {
  for (vf &vec : matrix) {
    float norm = Norm(vec);
    for (float &f : vec) {
      f = f / norm;
    }
  }
}

float RandomFloat() {
  return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

vvf GetRandomVector(int m, int n) {
  vvf temp(m, vf(n, 0.));
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      temp[i][j] = RandomFloat() * 2 - 1;
    }
  }
  NormalizeVectors(temp);
  return temp;
}


int main() {
  auto queries = GetRandomVector(NQUERIES, NDIM);
  auto documents = GetRandomVector(NDOCUMENTS, NDIM);

  vvpif res, res2;
  for (int _ = 0; _ < 1; _++) {
    auto a =
        duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    res = BFNN(queries, documents, 10);
    auto b =
        duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    cout << "time taken for bfnn: " << (b - a).count() << endl;
  }

  {
    NSW db;
    db.BuildIndex(documents);
    cout << "db created \n";
    float total = 0;
    for (int j = 0; j < 10; j++) {
      auto a =
          duration_cast<milliseconds>(system_clock::now().time_since_epoch());
      res2 = db.Search(queries);
      auto b =
          duration_cast<milliseconds>(system_clock::now().time_since_epoch());
      cout << "time taken for NSW: " << (b - a).count() << endl;
      total += (b - a).count();
    }
    printf("average %0.2f\n", total / 10);
  }
  // auto res3 =

  PrintMetrics(res, res2);

  // sample retrieved vectors
  cout << "From BFNN -> " << endl;
  for (auto v : res) {
    for (auto x : v) {
      printf("%.2f(%d) ", x.second, x.first);
    }
    cout << endl;
    break;
  }

  cout << "From NSW -> " << endl;
  for (auto v : res2) {
    for (auto x : v) {
      printf("%.2f(%d) ", x.second, x.first);
    }
    cout << endl;
    break;
  }
}