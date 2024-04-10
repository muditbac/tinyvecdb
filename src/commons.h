#ifndef COMMONS_H
#define COMMONS_H

#include <utility>
#include <vector>

std::ostream &get_null_stream();

#ifdef DEBUG
#define COUT_DEBUG std::cout
#else
#define COUT_DEBUG get_null_stream()
#endif

using std::pair;
using std::vector;

#define vvf vector<vector<float>>
#define vf vector<float>
#define vvpif vector<vector<pair<int, float>>>
#define vpif vector<pair<int, float>>
#define pif pair<int, float>

#endif