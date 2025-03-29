#include "RoutePlanner.h"
#include <fstream>
#include <queue>
#include <limits>
#include <sstream>

vector<int> RoutePlanner::dijkstraShortestPath(Graph<int>& g, int src, int dest, double& totalCost) {
    unordered_map<int, double> dist;
    unordered_map<int, int> prev;

    auto cmp = [&dist](int left, int right) { return dist[left] > dist[right]; };
    priority_queue<int, vector<int>, decltype(cmp)> pq(cmp);

    for (auto v : g.getVertexSet()) {
        int node = v->getInfo();
        dist[node] = numeric_limits<double>::infinity();
        prev[node] = -1;
    }

    dist[src] = 0.0;
    pq.push(src);

    unordered_set<int> visited;

    while (!pq.empty()) {
    int u = pq.top(); pq.pop();
    if (visited.count(u)) continue;
    visited.insert(u);

    if (u == dest) break;
    auto vu = g.findVertex(u);
    for (auto e : vu->getAdj()) {
        int v = e->getDest()->getInfo();
        double weight = e->getDrivingWeight();
        if (weight == INF) continue;

        if (dist[u] + weight < dist[v]) {
            dist[v] = dist[u] + weight;
            prev[v] = u;
            pq.push(v);
        }
    }
    }
    totalCost = dist[dest];

    vector<int> path;
    for (int at = dest; at != -1; at = prev[at]) {
        path.insert(path.begin(), at);
    }

    if (path.empty() || path.front() != src) {
        totalCost = INF;
        return {};
    }

    return path;
}

vector<int> RoutePlanner::findAlternativePath(Graph<int>& g, int src, int dest, const vector<int>& bestPath, double& totalCost2) {
    unordered_set<int> forbidden;
    for (size_t i = 1; i + 1 < bestPath.size(); ++i) {
        forbidden.insert(bestPath[i]);
    }

    unordered_map<int, double> dist;
    unordered_map<int, int> prev;

    auto cmp = [&dist](int left, int right) { return dist[left] > dist[right]; };
    priority_queue<int, vector<int>, decltype(cmp)> pq(cmp);

    for (auto v : g.getVertexSet()) {
        int id = v->getInfo();
        dist[id] = INF;
        prev[id] = -1;
    }

    dist[src] = 0;
    pq.push(src);

    unordered_set<int> visited;

    while (!pq.empty()) {
        int u = pq.top(); pq.pop();
        if (visited.count(u)) continue;
        visited.insert(u);

        if (u == dest) break;
        if (u != src && u != dest && forbidden.count(u)) continue;

        auto vu = g.findVertex(u);
        for (auto e : vu->getAdj()) {
            int v = e->getDest()->getInfo();
            double weight = e->getDrivingWeight();
            if (weight == INF) continue;
            if (v != src && v != dest && forbidden.count(v)) continue;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.push(v);
            }
        }
    }

    totalCost2 = dist[dest];

    vector<int> altPath;
    for (int at = dest; at != -1; at = prev[at])
        altPath.insert(altPath.begin(), at);

    if (altPath.empty() || altPath.front() != src || altPath.back() != dest){
        totalCost2 = INF;
        return {};
    }

    return altPath;
}
