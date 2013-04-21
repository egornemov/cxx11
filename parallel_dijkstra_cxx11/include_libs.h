#ifndef INCLUDE_LIBS_H
#define INCLUDE_LIBS_H

#include <vector>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <tuple>
#include <iostream>
#include <sstream>
#include <set>

const int uinfinity = -1;

template<typename WEIGHT = int>
using RoadMap = std::tuple<bool,std::vector<WEIGHT>,int>;

template<typename WEIGHT = int>
using GraphSolution = std::tuple<std::vector<int>,std::set<int>,int,WEIGHT,std::vector<WEIGHT>>;

#endif // INCLUDE_LIBS_H
