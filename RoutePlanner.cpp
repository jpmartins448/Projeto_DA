#include "RoutePlanner.h"
#include <fstream>
#include <queue>
#include <limits>
#include <sstream>

vector<int> RoutePlanner::dijkstraShortestPath(Graph<int>& g, int src, int dest) {
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
    cout << "Visiting node: " << u << " with dist: " << dist[u] << endl;
    if (visited.count(u)) continue;
    visited.insert(u);

    if (u == dest) break;
    auto vu = g.findVertex(u);
    for (auto e : vu->getAdj()) {
        int v = e->getDest()->getInfo();
        double weight = e->getDrivingWeight();
        if (weight == INF) continue;

        if (dist[u] + weight < dist[v]) {
            cout << "  Updating node " << v << ": new dist = " << dist[u] + weight
         << " (prev = " << dist[v] << "), prev node = " << u << endl;
            dist[v] = dist[u] + weight;
            prev[v] = u;
            pq.push(v);
        }
    }
}

    vector<int> path;
    for (int at = dest; at != -1; at = prev[at]) {
        path.insert(path.begin(), at);
    }

    if (path.empty() || path.front() != src)
        return {};

    return path;
}
