#ifndef ROUTE_PLANNER_H
#define ROUTE_PLANNER_H

#include "Graph.h"
#include <vector>
#include <unordered_set>
#include <string>

using namespace std;

class RoutePlanner {
public:
    static vector<int> dijkstraShortestPath(Graph<int>& g, int src, int dest);
    static void runT2_1(Graph<int>& g, const string& inputFile, const string& outputFile);
};

#endif  