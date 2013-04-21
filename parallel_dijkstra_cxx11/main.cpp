#include "gen_graph.h"
#include "include_libs.h"

template<typename WEIGHT = int>
auto findPath(const std::vector<WEIGHT>& roadMap, int size, int pos = 0)->GraphSolution<WEIGHT>
{
	std::vector<int> readyVertices{pos};
    std::vector<WEIGHT> edgeWeights(size);
	std::set<int> unreadyVertices{};
	auto getCounter = [size]()->int {
		static int curVertix = 0;
		if(curVertix == size) curVertix = 0;
		return curVertix++;
	};
	auto defineVertices = [&unreadyVertices,size, pos]() {
		for(int i = 0; i < size; ++i) if(i != pos) unreadyVertices.insert(i);
	};
    auto defineWeights = [size,pos,getCounter](int)->WEIGHT {
        WEIGHT curWeight = uinfinity;
		if(getCounter() == pos) curWeight = 0;
		return curWeight;
	};
	defineVertices();
	std::transform(edgeWeights.begin(),edgeWeights.end(),edgeWeights.begin(), defineWeights);
    GraphSolution<WEIGHT> tmpSolution{readyVertices,unreadyVertices,pos,0,edgeWeights};

    auto addIfInf = [](WEIGHT base,WEIGHT val)->WEIGHT {
		if(val == uinfinity) return uinfinity;
		return base + val;
	};

    [=,&tmpSolution]()mutable {
		for(int i = 0; i < size; ++i) std::get<4>(tmpSolution)[i] = roadMap[pos*size + i];

        while(!std::get<1>(tmpSolution).empty()) {

			// Define shortest path
            int old_pos = pos;

            WEIGHT min_weight = std::get<3>(tmpSolution);

            if(!min_weight) min_weight = uinfinity;

			for(auto& elem: std::get<1>(tmpSolution)) {
                WEIGHT tmp_weigth = addIfInf(std::get<3>(tmpSolution),roadMap[old_pos*size + elem]);


				if(tmp_weigth == uinfinity)
					tmp_weigth = std::get<4>(tmpSolution)[elem];

				if(std::get<4>(tmpSolution)[elem] != uinfinity) {
					if(tmp_weigth < std::get<4>(tmpSolution)[elem])
							std::get<4>(tmpSolution)[elem] = tmp_weigth;
				}
				else std::get<4>(tmpSolution)[elem] = tmp_weigth;


				if(elem == *(std::get<1>(tmpSolution).begin()))
				{
					min_weight = std::get<4>(tmpSolution)[elem];
                    pos = elem;
				}
				else if(tmp_weigth != uinfinity && tmp_weigth < min_weight) {
					min_weight = tmp_weigth;
                    pos = elem;
				}

			}

			std::get<3>(tmpSolution) = min_weight;
            std::get<2>(tmpSolution) = pos;
            std::get<1>(tmpSolution).erase(pos);
            std::get<0>(tmpSolution).push_back(pos);

            std::cout << __LINE__ << " " << pos << std::endl;

		}
	}();

	return tmpSolution;
}

int main()
{
	int vertixN{5}, edgeN{10};
	float base{100}; // variable base defines max value (below uinfinity) of edge weight
    RoadMap<decltype(base)> roadMap;
	GraphSolution<decltype(base)> graphSolution;

	auto const printMat = [](std::vector<decltype(base)> vec, int size){
		int curPos = 0;
		for(auto elem: vec)
		{
			std::cout << elem << " ";
			if(elem < 10 && elem >= 0) std::cout << " ";
			if(!(++curPos % size)) std::cout << std::endl;
		}
	};

	auto clearRoadMap = [&roadMap](){
		std::get<0>(roadMap) = false;
		std::get<1>(roadMap).clear();
		std::get<2>(roadMap) = 0;
	};

	auto clearGraphSolution = [&graphSolution](){
		std::get<0>(graphSolution).clear();
		std::get<1>(graphSolution).clear();
		std::get<2>(graphSolution) = 0;
		std::get<3>(graphSolution) = 0;
		std::get<4>(graphSolution).clear();
	};

	auto vecIntToStr = [](std::vector<int> vec)->std::string {
		std::ostringstream oss;
		for(auto elem: vec) oss << " " << elem;
		return oss.str();
	};

	auto vecBaseToStr = [](std::vector<decltype(base)> vec)->std::string {
		std::ostringstream oss;
		for(auto elem: vec) oss << " " << elem;
		return oss.str();
	};

	auto setToStr = [](std::set<int> vec)->std::string {
		std::ostringstream oss;
		for(auto elem: vec) oss << " " << elem;
		return oss.str();
	};

	auto printSolution = [=](const decltype(graphSolution)& solution){
		for(int i = 0; i != 5; ++i)
			std::cout << i << ")" << [=]()->std::string {
				std::ostringstream oss;
				switch(i){
                case(0) : oss << vecIntToStr(std::get<0>(solution)); break;
				case(1) : oss << setToStr(std::get<1>(solution)); break;
				case(2) : oss << " " << std::get<2>(solution); break;
				case(3) : oss << " " << std::get<3>(solution); break;
                case(4) : oss << vecBaseToStr(std::get<4>(solution)); break;
				}
				return oss.str();
			}() << "; " << std::endl;
		std::cout << std::endl;
	};

//	auto genEdgeN = [=,&edgeN](int& i){
//		edgeN = (i + 1)*10;
//		if(edgeN > vertixN*(vertixN - 1)) i = uinfinity;
//	};

    vertixN = 6;
    edgeN = (vertixN*vertixN)>>1;
	clearRoadMap();
	roadMap = genGraph<decltype(base)>(vertixN, edgeN, base);
	if(std::get<0>(roadMap))
	{
		std::cout << "Number of edges: expected " << edgeN << "; real " << std::get<2>(roadMap) << std::endl;
        if(vertixN < 25) printMat(std::get<1>(roadMap), vertixN);
			clearGraphSolution();
			graphSolution = findPath<decltype(base)>(std::get<1>(roadMap), vertixN);
			printSolution(graphSolution);
	}

//	for(int i = 0; i < 20; ++i){
//		vertixN += 2;
//		edgeN = (vertixN*vertixN)>>1;
//		clearRoadMap();
//		roadMap = genGraph<decltype(base)>(vertixN, edgeN, base);
//		if(std::get<0>(roadMap))
//		{
//			std::cout << "Number of edges: expected " << edgeN << "; real " << std::get<2>(roadMap) << std::endl;
//			if(vertixN < 15) printMat(std::get<1>(roadMap), vertixN);
//				clearGraphSolution();
//				graphSolution = findPath<decltype(base)>(std::get<1>(roadMap), vertixN);
//				printSolution(graphSolution);
//		}
//	}

//	for(int i = 0; i != uinfinity; ++i, genEdgeN(i))
//	{
//		clearRoadMap();
//		roadMap = genGraph<decltype(base)>(vertixN, edgeN, base);
//		if(std::get<0>(roadMap))
//		{
//			std::cout << "Number of edges: expected " << edgeN << "; real " << std::get<2>(roadMap) << std::endl;
//			printMat(std::get<1>(roadMap), vertixN);
//			clearGraphSolution();
//			graphSolution = findPath<decltype(base)>(std::get<1>(roadMap), vertixN);
//			printSolution(graphSolution);
//		}
//	}

	return 0;
}

