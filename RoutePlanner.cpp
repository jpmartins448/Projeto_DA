#include "RoutePlanner.h"
#include <fstream>
#include <queue>
#include <limits>
#include <sstream>
#include <set>


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

vector<tuple<vector<int>, int, vector<int>, int>> RoutePlanner::findApproximateDriveWalkRoutes(
    Graph<int>& g, int source, int destination, int initialMaxWalkTime,
    const unordered_set<int>& avoidNodes,
    const unordered_set<pair<int, int>, pair_hash>& avoidSegments
) {
    vector<tuple<vector<int>, int, vector<int>, int>> allCandidates;
    set<tuple<vector<int>, int, vector<int>>> seen;  // (drivePath, parkingNode, walkPath)

    // Explore increasing MaxWalkTime up to +30

        for (Vertex<int>* p : g.getVertexSet()) {
            int parkingId = p->getInfo();
            if (!p->getParking() || parkingId == source || parkingId == destination)
                continue;

            auto [drivePath, driveTime] = dijkstra(g, source, parkingId, false, avoidNodes, avoidSegments);
            if (drivePath.size() < 1) continue;

            auto [walkPath, walkTime] = dijkstra(g, parkingId, destination, true, avoidNodes, avoidSegments);
            if (walkPath.size() < 1 ) continue;

            // Skip if we've seen this exact configuration
            if (!seen.insert({drivePath, parkingId, walkPath}).second) continue;

            int total = static_cast<int>(driveTime + walkTime);
            allCandidates.emplace_back(drivePath, parkingId, walkPath, total);
        }

        for (size_t i = 0; i < allCandidates.size(); i++) {
            auto temp = allCandidates[i];
        }

    // Sort all found options by total travel time (4th element in tuple)
    sort(allCandidates.begin(), allCandidates.end(), [](const auto& a, const auto& b) {
        return get<3>(a) < get<3>(b);
    });

    // Return only the 2 best alternatives
    if (allCandidates.size() > 2)
        allCandidates.resize(2);

    return allCandidates;
}


pair<vector<int>, double> RoutePlanner::dijkstra(
    Graph<int>& g, int src, int dst, bool walking,
    const unordered_set<int>& avoidNodes,
    const unordered_set<pair<int, int>, pair_hash>& avoidSegments) {
    
    unordered_map<int, double> dist;
    unordered_map<int, int> prev;
    auto cmp = [&dist](int a, int b) { return dist[a] > dist[b]; }; //same logic as before
    priority_queue<int, vector<int>, decltype(cmp)> pq(cmp);

    for (auto v : g.getVertexSet()) {
        dist[v->getInfo()] = INF;
        prev[v->getInfo()] = -1;
    }

    dist[src] = 0.0;
    pq.push(src);

    while (!pq.empty()) {
        int u = pq.top(); pq.pop();
        if (avoidNodes.count(u)) continue; //if a node is to be avoided, ignore it (same logic as ignoring the best path when doing an alternative one)
        if (u == dst) break;

        Vertex<int>* vu = g.findVertex(u);
        for (Edge<int>* e : vu->getAdj()) {
            int v = e->getDest()->getInfo();
            if (avoidNodes.count(v)) continue; //again, if it is to be avoided, ignore it
            if (avoidSegments.count({u, v})) continue; //stored segments into a set of pairs so we know which ones to ignore

            double weight = walking ? e->getWalkingWeight() : e->getDrivingWeight(); //this way we can tell the algorithm when we call it if we want the walking weight or the driving
            if (weight == INF) continue;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.push(v);
            }
        }
    }

    vector<int> path;
    for (int at = dst; at != -1; at = prev[at]) { //reconstruct the path
        path.insert(path.begin(), at);
    }

    if (path.empty() || path.front() != src) return {{}, INF};
    return {path, dist[dst]};
}

tuple<vector<int>, int, vector<int>, int, string> RoutePlanner::findBestDriveWalkRoute(
    Graph<int>& g, int source, int destination, int maxWalkTime,
    const unordered_set<int>& avoidNodes,
    const unordered_set<pair<int, int>, pair_hash>& avoidSegments
) {
    vector<int> bestDrivePath, bestWalkPath;
    int bestParkingNode = -1;
    double minTotalTime = INF;
    double bestWalkTime = -1;

    bool routeExistsIgnoringWalkLimit = false;

    for (Vertex<int>* p : g.getVertexSet()) {
        int parkingId = p->getInfo();
        if (!p->getParking() || parkingId == source || parkingId == destination)
            continue;

        auto [drivePath, driveTime] = dijkstra(g, source, parkingId, false, avoidNodes, avoidSegments);
        if (drivePath.size() < 2) continue;

        auto [walkPath, walkTime] = dijkstra(g, parkingId, destination, true, avoidNodes, avoidSegments);
        if (walkPath.size() < 2) continue;

        routeExistsIgnoringWalkLimit = true;  // A route exists, but maybe not valid due to maxWalkTime

        if (walkTime > maxWalkTime) continue;

        double totalTime = driveTime + walkTime;
        if (totalTime < minTotalTime || 
            (totalTime == minTotalTime && walkTime > bestWalkTime)) {
            minTotalTime = totalTime;
            bestWalkTime = walkTime;
            bestDrivePath = drivePath;
            bestWalkPath = walkPath;
            bestParkingNode = parkingId;
        }
    }

    if (bestDrivePath.empty()) {
        string message;
        if (!routeExistsIgnoringWalkLimit)
            message = "No route found that satisfies driving and walking constraints (considering avoidNodes and avoidSegments).";
        else
            message = "No route found with walking time under the allowed maxWalkTime.";

        return {{}, -1, {}, static_cast<int>(INF), message};
    }

    return {
        bestDrivePath,
        bestParkingNode,
        bestWalkPath,
        static_cast<int>(minTotalTime),
        ""
    };
}