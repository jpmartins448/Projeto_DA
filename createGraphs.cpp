#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>

using namespace std;

#include "./createGraphs.h"

vector<int> createGraphs::findIsolatedNodesInGraph(Graph<int> g) {
    int i, id;
    vector<int> res;
    vector<int> rem;

    for (auto v : g.getVertexSet()) {
        if (v->getAdj().empty()) {
            res.push_back(v->getInfo());
        }
    }

    for (auto v : g.getVertexSet()) {
        for (auto & e : v->getAdj()) {
            auto w = e->getDest();
            auto it = std::find(res.begin(), res.end(), w->getInfo());
            if (it != res.end()) {
                rem.push_back(w->getInfo());
            }
        }
    }

    for(i = 0; i < rem.size(); i++){
        id = rem[i];
        auto it = std::find(res.begin(), res.end(), id);
        if(it != res.end())
            res.erase(it);
    }
    return res;
}


string trim(const string &s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

Graph<int> createGraphs::createGraphFromCSV(const string& locationsFile, const string& distancesFile) {
    Graph<int> g;
    unordered_map<string, int> codeToId;
    unordered_map<int, bool> hasParking;

    // Load locations.csv
    ifstream locFile(locationsFile);
    if (!locFile.is_open()) {
        cerr << "Failed to open " << locationsFile << endl;
        return g;
    }

    string line;
    getline(locFile, line); // Skip header

    while (getline(locFile, line)) {
        stringstream ss(line);
        string name, idStr, code, parkingStr;
        getline(ss, name, ',');
        getline(ss, idStr, ',');
        getline(ss, code, ',');
        getline(ss, parkingStr, ',');

        int id = stoi(idStr);
        int parking = stoi(parkingStr);

        g.addVertex(id);
        codeToId[code] = id;
        hasParking[id] = (parking == 1);

        Vertex<int>* v = g.findVertex(id);
        if (v) {
            v->setInfo(id);
            v->setLocation(name);
            v->setParking(parking == 1);
            v->setCode(code);
        }
    }

    locFile.close();

    // Load distances.csv
    ifstream distFile(distancesFile);
    if (!distFile.is_open()) {
        cerr << "Failed to open " << distancesFile << endl;
        return g;
    }

    getline(distFile, line); // Skip header

    while (getline(distFile, line)) {
        stringstream ss(line);
        string code1, code2, driveStr, walkStr;
        getline(ss, code1, ',');
        getline(ss, code2, ',');
        getline(ss, driveStr, ',');
        getline(ss, walkStr, ',');

        driveStr = trim(driveStr);
        walkStr = trim(walkStr);

        if (codeToId.find(code1) == codeToId.end() || codeToId.find(code2) == codeToId.end())
            continue;

        int id1 = codeToId[code1];
        int id2 = codeToId[code2];

        double driveTime = INF;
        double walkTime = INF;

        bool canDrive = (driveStr != "X");
        bool canWalk = (walkStr != "X");

        if (canDrive) driveTime = stod(driveStr);
        if (canWalk) walkTime = stod(walkStr);

        // Add bidirectional edge
        if (g.findVertex(id1) && g.findVertex(id2)) {
            auto added = g.addBidirectionalEdge(id1, id2, driveTime);
            if (added) {
                Vertex<int>* v1 = g.findVertex(id1);
                Vertex<int>* v2 = g.findVertex(id2);

                for (Edge<int>* e : v1->getAdj()) {
                    if (e->getDest() == v2) {
                        e->setWalkingWeight(canWalk ? walkTime : INF);
                    }
                }

                for (Edge<int>* e : v2->getAdj()) {
                    if (e->getDest() == v1) {
                        e->setWalkingWeight(canWalk ? walkTime : INF);
                    }
                }
            }
        }
    }

    distFile.close();
    return g;
}



Graph<int> createGraphs::graph1() {
    Graph<int> g;
    for (int i = 0; i <= 8; i++){
        g.addVertex(i);
    }

    g.addEdge(0,1,0);
    g.addEdge(0,2,0);
    g.addEdge(1,3,0);
    g.addEdge(2,3,0);
    g.addEdge(3,4,0);
    g.addEdge(4,5,0);
    g.addEdge(4,6,0);
    g.addEdge(5,8,0);
    g.addEdge(6,7,0);

    return g;
}

Graph<int> createGraphs::graph2() {
    Graph<int> g;
    for (int i = 0; i <= 8; i++){
        g.addVertex(i);
    }

    g.addEdge(0,1,0);
    g.addEdge(0,2,0);
    g.addEdge(1,2,0);
    g.addEdge(3,4,0);
    g.addEdge(5,8,0);
    g.addEdge(5,6,0);
    g.addEdge(7,6,0);
    g.addEdge(7,8,0);

    return g;
}

Graph<int> createGraphs::graph3() {
    Graph<int> g;
    for (int i = 0; i <= 11; i++){
        g.addVertex(i);
    }

    g.addEdge(0,1,0);
    g.addEdge(1,2,0);
    g.addEdge(2,3,0);
    g.addEdge(0,6,0);
    g.addEdge(1,7,0);
    g.addEdge(3,8,0);
    g.addEdge(6,2,0);
    g.addEdge(7,8,0);
    g.addEdge(4,5,0);
    g.addEdge(4,10,0);
    g.addEdge(4,11,0);
    g.addEdge(5,9,0);
    g.addEdge(5,11,0);
    g.addEdge(11,10,0);
    g.addEdge(10,5,0);

    return g;
}

Graph<int> createGraphs::graph4() {
    Graph<int> g;
    for (int i = 0; i <= 7; i++){
        g.addVertex(i);
    }

    g.addEdge(0,1,0);
    g.addEdge(0,2,0);
    g.addEdge(1,3,0);
    g.addEdge(2,3,0);
    g.addEdge(3,4,0);
    g.addEdge(4,5,0);
    g.addEdge(8,5,0);
    g.addEdge(6,4,0);
    g.addEdge(7,6,0);

    return g;
}

Graph<int> createGraphs::graph5() {
    Graph<int> g;
    for (int i = 0; i <= 3; i++){
        g.addVertex(i);
    }

    g.addEdge(0,1,0);
    g.addEdge(1,3,0);
    g.addEdge(0,2,0);
    g.addEdge(2,3,0);

    return g;
}

Graph<int> createGraphs::graph6() {
    Graph<int> g;
    for (int i = 0; i <= 7; i++){
        g.addVertex(i);
    }

    g.addEdge(0, 1, 0);
    g.addEdge(1, 3, 0);
    g.addEdge(2, 0, 0);
    g.addEdge(3, 2, 0);
    g.addEdge(3, 4, 0);
    g.addEdge(4, 5, 0);
    g.addEdge(4, 6, 0);
    g.addEdge(5, 7, 0);
    g.addEdge(6, 2, 0);
    g.addEdge(7, 6, 0);

    return g;
}

Graph<int> createGraphs::graph7() {
    Graph<int> g;

    for (int i = 0; i <= 2; i++){
        g.addVertex(i);
    }

    g.addEdge(0, 1, 0);
    g.addEdge(1, 2, 0);
    g.addEdge(2, 0, 0);

    return g;
}

Graph<int> createGraphs::graph8() {
    Graph<int> g;
    for (int i = 0; i <= 10; i++){
        g.addVertex(i);
    }

    g.addEdge(0,1,0);
    g.addEdge(1,2,0);
    g.addEdge(2,3,0);
    g.addEdge(0,6,0);
    g.addEdge(1,7,0);
    g.addEdge(1,4,0);
    g.addEdge(3,8,0);
    g.addEdge(6,2,0);
    g.addEdge(7,8,0);
    g.addEdge(4,5,0);
    g.addEdge(4,10,0);
    g.addEdge(5,9,0);
    g.addEdge(5,10,0);

    return g;
}

// ------------------------------------------------------------
Graph<int> createGraphs::graph9() {
    Graph<int> g;
    for (int i = 0; i <= 12; i++){
        g.addVertex(i);
    }

    g.addEdge(0,1,0);
    g.addEdge(1,2,0);
    g.addEdge(2,3,0);
    g.addEdge(0,6,0);
    g.addEdge(1,7,0);
    g.addEdge(1,4,0);
    g.addEdge(3,8,0);
    g.addEdge(6,2,0);
    g.addEdge(7,8,0);
    g.addEdge(4,5,0);
    g.addEdge(4,10,0);
    g.addEdge(5,9,0);
    g.addEdge(5,10,0);
    g.addEdge(9,11,0);
    g.addEdge(10,11,0);
    g.addEdge(8,12,0);
    g.addEdge(11,12,0);
    g.addEdge(11,4,0);

    return g;
}

// ------------------------------------------------------------
Graph<int> createGraphs::graph10() {
    Graph<int> g;
    for (int i = 0; i <= 7; i++){
        g.addVertex(i);
    }

    g.addEdge(0,1,0);
    g.addEdge(1,2,0);
    g.addEdge(3,2,0);
    g.addEdge(4,1,0);
    g.addEdge(2,5,0);
    g.addEdge(5,6,0);
    g.addEdge(7,6,0);

    return g;
}

// ------------------------------------------------------------
Graph<int> createGraphs::graph11() {
    Graph<int> g;
    for (int i = 0; i <= 5; i++){
        g.addVertex(i);
    }

    g.addEdge(0,1,0);
    g.addEdge(0,2,0);
    g.addEdge(0,3,0);
    g.addEdge(0,4,0);
    g.addEdge(0,5,0);

    return g;
}

// ------------------------------------------------------------
Graph<int> createGraphs::graph12() {
    Graph<int> g;
    for (int i = 0; i <= 5; i++){
        g.addVertex(i);
    }

    g.addEdge(1,0,0);
    g.addEdge(2,0,0);
    g.addEdge(3,0,0);
    g.addEdge(4,0,0);
    g.addEdge(5,0,0);

    return g;
}

// ------------------------------------------------------------
Graph<int> createGraphs::graph13() {
    Graph<int> g;
    for (int i = 0; i <= 5; i++){
        g.addVertex(i);
    }

    g.addEdge(1,2,0);
    g.addEdge(2,3,0);
    g.addEdge(3,4,0);
    g.addEdge(4,5,0);
    g.addEdge(5,1,0);

    return g;
}

// ------------------------------------------------------------
Graph<int> createGraphs::graph14() {
    Graph<int> g;
    for (int i = 0; i <= 5; i++){
        g.addVertex(i);
    }

    g.addEdge(0,1,0);
    g.addEdge(2,1,0);
    g.addEdge(3,1,0);
    g.addEdge(4,1,0);
    g.addEdge(5,1,0);
    return g;
}

// ------------------------------------------------------------
Graph<int> createGraphs::graph15() {
    Graph<int> g;
    for (int i = 0; i <= 2; i++){
        g.addVertex(i);
    }

    g.addEdge(1,0,0);
    g.addEdge(0,2,0);
    g.addEdge(1,2,0);
    return g;
}

// ------------------------------------------------------------
Graph<int> createGraphs::graph16() {
    Graph<int> g;
    for (int i = 0; i <= 5; i++){
        g.addVertex(i);
    }

    g.addEdge(1,0,0);
    g.addEdge(0,2,0);
    g.addEdge(3,4,0);
    g.addEdge(4,5,0);
    return g;
}

// ------------------------------------------------------------
Graph<int> createGraphs::graph17() {
    Graph<int> g;

    for (int i = 0; i <= 5; i++){
        g.addVertex(i);
    }

    g.addEdge(0,3,0);
    g.addEdge(0,1,0);
    g.addEdge(1,2,0);
    g.addEdge(3,4,0);
    g.addEdge(4,5,0);
    g.addEdge(3,2,0);
    return g;
}

// ------------------------------------------------------------
Graph<int> createGraphs::graph18() {
    Graph<int> g;
    for (int i = 0; i <= 5; i++){
        g.addVertex(i);
    }

    g.addEdge(0,1,0);
    g.addEdge(1,2,0);
    g.addEdge(2,1,0);
    g.addEdge(3,4,0);
    g.addEdge(4,5,0);
    g.addEdge(5,4,0);
    return g;
}

// ------------------------------------------------------------
Graph<int> createGraphs::graph19() {
    Graph<int> g;
    for (int i = 0; i <= 8; i++){
        g.addVertex(i);
    }

    g.addEdge(3,1,0);
    g.addEdge(3,2,0);
    g.addEdge(1,0,0);
    g.addEdge(2,0,0);
    g.addEdge(0,4,0);
    g.addEdge(4,5,0);
    g.addEdge(4,6,0);
    g.addEdge(5,8,0);
    g.addEdge(6,7,0);
    return g;
}

// ------------------------------------------------------------
Graph<int> createGraphs::graph20() {
    Graph<int> g;
    for (int i = 0; i <= 8; i++){
        g.addVertex(i);
    }

    g.addEdge(0,1,0);
    g.addEdge(2,0,0);
    g.addEdge(1,2,0);
    g.addEdge(4,2,0);
    g.addEdge(3,4,0);
    g.addEdge(5,8,0);
    g.addEdge(5,6,0);
    g.addEdge(7,6,0);
    g.addEdge(7,8,0);
    return g;
}

Graph<int> createGraphs::createGraphNumber(int n) {
    Graph<int> g;

    switch(n){
        case 1:  g = createGraphs::graph1(); break;
        case 2:  g = createGraphs::graph2(); break;
        case 3:  g = createGraphs::graph3(); break;
        case 4:  g = createGraphs::graph4(); break;
        case 5:  g = createGraphs::graph5(); break;
        case 6:  g = createGraphs::graph6(); break;
        case 7:  g = createGraphs::graph7(); break;
        case 8:  g = createGraphs::graph8(); break;
        case 9:  g = createGraphs::graph9(); break;
        case 10: g = createGraphs::graph10(); break;
        case 11: g = createGraphs::graph11(); break;
        case 12: g = createGraphs::graph12(); break;
        case 13: g = createGraphs::graph13(); break;
        case 14: g = createGraphs::graph14(); break;
        case 15: g = createGraphs::graph15(); break;
        case 16: g = createGraphs::graph16(); break;
        case 17: g = createGraphs::graph17(); break;
        case 18: g = createGraphs::graph18(); break;
        case 19: g = createGraphs::graph19(); break;
        case 20: g = createGraphs::graph20(); break;
        default: g = createGraphs::graph1(); break;
    }
    return g;
}

// ------------------------------------------------------------
//
// ------------------------------------------------------------

void createGraphs::emitDOTFile(string gname, Graph<int> g) {
    ofstream g_dot_file;
    int idx;
    vector<int> rem;

    g_dot_file.open (gname+".gv");
    g_dot_file << "digraph " << gname << "{\n";


    g_dot_file << "label     = \"Graph: " << gname << "\";\n";
    g_dot_file << "labelloc  = top;\n";
    g_dot_file << "labeljust = left;\n";
    g_dot_file << "fontname  = calibri;\n";
    g_dot_file << "fontsize  = 16;\n";
    g_dot_file << "\n";

    rem = findIsolatedNodesInGraph(g);

    for (auto v : g.getVertexSet()) {
        idx = v->getInfo();
        auto it = std::find(rem.begin(), rem.end(), idx);
        if (it != rem.end()) {
            g_dot_file << "  " << idx << " -> " << idx << " [color = \"white\"];\n";
        } else {
            for (auto & e : v->getAdj()) {
                auto w = e->getDest();
                g_dot_file << "  " << v->getInfo() << " -> " << w->getInfo() << ";";
                g_dot_file << "\n";
            }
        }
    }

    g_dot_file << "}\n";
    g_dot_file.close();
}


// ------------------------------------------------------------
