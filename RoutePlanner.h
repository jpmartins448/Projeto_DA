#ifndef ROUTE_PLANNER_H
#define ROUTE_PLANNER_H

#include "Graph.h"
#include <vector>
#include <unordered_set>
#include <string>
#include <unordered_map>

using namespace std;

struct pair_hash {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const {
        auto h1 = hash<T1>{}(p.first);
        auto h2 = hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);  // basic combination of hashes
    }
};

class RoutePlanner {
public:
    static vector<int> dijkstraShortestPath(Graph<int>& g, int src, int dest, double& totalCost);
    static vector <int> findAlternativePath(Graph<int>& g, int src, int dest, const vector<int>& bestPath, double& totalCost2);
    static pair<vector<int>, double> dijkstra(Graph <int>& g, int src, int dst, bool walking,
        const unordered_set<int>& avoidNodes,
        const unordered_set<pair<int, int>, pair_hash>& avoidSegments);
    static tuple<vector<int>, int, vector<int>, int, string> findBestDriveWalkRoute(Graph<int>& g, int source, int destination, int maxWalkTime,
            const unordered_set<int>& avoidNodes,
            const unordered_set<pair<int, int>, pair_hash>& avoidSegments);
    static vector<tuple<vector<int>, int, vector<int>, int>> findApproximateDriveWalkRoutes(
                Graph<int>& g, int source, int destination, int initialMaxWalkTime,
                const unordered_set<int>& avoidNodes,
                const unordered_set<pair<int, int>, pair_hash>& avoidSegments);
};

#endif  