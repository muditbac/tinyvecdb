
#include "debug.h"
#include <iostream>

using std::cout, std::endl;

void PrintResultSample(vvpif res) {
  for (auto query : res) {
    for (auto t : query) {
      cout << t.first << " " << t.second << " | ";
    }
    cout << endl;
    break;
  }
}