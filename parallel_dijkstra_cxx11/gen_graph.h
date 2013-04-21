#ifndef GEN_GRAPH_H
#define GEN_GRAPH_H

#include "include_libs.h"

template<typename WEIGHT = int>
auto genGraph(int vertixN, int edgeN, WEIGHT base = std::numeric_limits<WEIGHT>::max())->RoadMap<WEIGHT>
{
    std::vector<WEIGHT> roadMap;
	roadMap.resize(vertixN*vertixN);
	int realEdgeN = 0;

	int curPos = 0;
    auto markCells = [=,&realEdgeN](WEIGHT)mutable->WEIGHT {
        WEIGHT elem = uinfinity;
		if(curPos == (curPos % vertixN)*(vertixN + 1)) elem = 0;
		else {
			int tmp_elem = rand() % (vertixN*vertixN) + 1;
            elem = (tmp_elem > edgeN) ? elem : [=,&realEdgeN]()->WEIGHT {
				++realEdgeN; return tmp_elem;}();
		}
		++curPos;
		return elem;
	};

	std::transform(roadMap.begin(), roadMap.end(), roadMap.begin(), markCells);

    auto diceRoll = [=]()->WEIGHT {
        return std::abs(std::rand() % (int)base + 1);
	};

    auto fillMat = [=](WEIGHT& elem) {
		if(elem && elem != uinfinity) elem = diceRoll();
	};

	for(auto& elem: roadMap)
	  fillMat(elem);

    return RoadMap<WEIGHT>(true, roadMap, realEdgeN);
//	return std::make_tuple(true, roadMap, realEdgeN);
}

#endif // GEN_GRAPH_H
