#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>
#include "createGraphs.h"
#include "RoutePlanner.h"

using namespace std;

void displayMenu() {
    //this method displays the menu to the user
    cout << "====== Route Planning Tool ======\n";
    cout << "1. Independent Route Planning\n";
    cout << "2. Restricted Route Planning\n";
    cout << "3. Environmentally-Friendly Route Planning (driving and walking)\n";
    cout << "0. Exit\n";
    cout << "Enter your choice: ";
}


void planRoute(Graph<int>& g, string start, string end) {
    cout << "Source:" << start << "\nDestination:" << end << "\n";
    // Convert source and destination to integers
    int src = stoi(start);
    int dst = stoi(end);

    double totalCost = 0;
    
    vector<int> bestPath = RoutePlanner::dijkstraShortestPath(g, src, dst, totalCost); //creates a vector with the shortest path

    if (bestPath.empty()) {
        cout << "BestDrivingRoute:none\n";
        cout << "AlternativeDrivingRoute:none\n";
        return;
    }

    cout << "BestDrivingRoute:";
    for (size_t i = 0; i < bestPath.size(); ++i) {
        //outputs the elements in the vector (shortest-path)
        cout << bestPath[i];
        if (i != bestPath.size() - 1) cout << ",";
    }
    cout << "(" << totalCost << ")\n";

    double totalCost2 = 0;

    vector<int> altPath = RoutePlanner::findAlternativePath(g, src, dst, bestPath, totalCost2); 
    //creates a vector with the alternative path

    if (altPath.empty()) {
        cout << "AlternativeDrivingRoute:none\n";
        return;
    }


    cout << "AlternativeDrivingRoute:";
    for (size_t i = 0; i < altPath.size(); ++i) {
        //outputs the elements of the vector containing the alternative path
        cout << altPath[i];
        if (i != altPath.size() - 1) cout << ",";
    }
    cout << "(" << totalCost2 << ")\n";
}


//same logic as planRoute() but insatead of printing in the terminal, it prints in the output.txt
void planRoute_(Graph<int>& g, string start, string end, ofstream &output) {
    
    output << "Source:" << start << "\nDestination:" << end << "\n";
    int src = stoi(start);
    int dst = stoi(end);

    double totalCost = 0;
    
    vector<int> bestPath = RoutePlanner::dijkstraShortestPath(g, src, dst, totalCost);

    if (bestPath.empty()) {
        output << "BestDrivingRoute:none\n";
        output << "AlternativeDrivingRoute:none\n";
        return;
    }

    output << "BestDrivingRoute:";
    for (size_t i = 0; i < bestPath.size(); ++i) {
        output << bestPath[i];
        if (i != bestPath.size() - 1) output << ",";
    }
    output << "(" << totalCost << ")\n";

    double totalCost2 = 0;

    vector<int> altPath = RoutePlanner::findAlternativePath(g, src, dst, bestPath, totalCost2);

    if (altPath.empty()) {
        output << "AlternativeDrivingRoute:none\n";
        return;
    }


    output << "AlternativeDrivingRoute:";
    for (size_t i = 0; i < altPath.size(); ++i) {
        output << altPath[i];
        if (i != altPath.size() - 1) output << ",";
    }
    output << "(" << totalCost2 << ")\n";
}


void planRestrictedRoute(string source, string destination, set<string> avoidNodes, set<pair<string, string>> avoidSegments, string includeNode) {
    cout << "Source:" << source << "\nDestination: " << destination << "\n";
    // Convert source and destination to integers
    int src = stoi(source);
    int dst = stoi(destination);

    // Create a restricted graph with the avoided nodes and segments removed
    Graph<int> restrictedGraph = createGraphs::createGraphFromCSV("LocationsTest.csv", "DistancesTest.csv");  // Assume using CSV files for graph

    // Remove nodes from the restricted graph
    for (const auto& node : avoidNodes) {
        restrictedGraph.removeVertex(stoi(node)); // Remove node by ID
    }

    // Remove segments from restricted graph
    for (const auto& seg : avoidSegments) {
        restrictedGraph.removeEdge(stoi(seg.first), stoi(seg.second)); // Remove segments by node pair
    }

    // Check if includeNode is not empty and run the two Dijkstra's
    if (!includeNode.empty()) {
        double totalCost1 = 0;
        // Run Dijkstra from source to includeNode
        vector<int> pathToIncludeNode = RoutePlanner::dijkstraShortestPath(restrictedGraph, src, stoi(includeNode), totalCost1);

        if (pathToIncludeNode.empty()) {
            cout << "BestDrivingRoute:none\n";
            cout << "AlternativeDrivingRoute:none\n";
            return;
        }

        // Run Dijkstra from includeNode to destination
        double totalCost2 = 0;
        vector<int> pathFromIncludeNode = RoutePlanner::dijkstraShortestPath(restrictedGraph, stoi(includeNode), dst, totalCost2);

        if (pathFromIncludeNode.empty()) {
            cout << "BestDrivingRoute:none\n";
            cout << "AlternativeDrivingRoute:none\n";
            return;
        }

        // Combine the two paths (source -> includeNode, includeNode -> destination)
        pathToIncludeNode.insert(pathToIncludeNode.end(), pathFromIncludeNode.begin() + 1, pathFromIncludeNode.end());

        cout << "BestDrivingRoute:";
        for (size_t i = 0; i < pathToIncludeNode.size(); ++i) {
            // Prints the shortest path
            cout << pathToIncludeNode[i];
            if (i != pathToIncludeNode.size() - 1) cout << ",";
        }
        cout << "(" << totalCost1 + totalCost2 << ")\n";
    } else {
        // If includeNode is empty, just find the shortest path without including a specific node
        double totalCost = 0;
        vector<int> bestPath = RoutePlanner::dijkstraShortestPath(restrictedGraph, src, dst, totalCost);

        if (bestPath.empty()) {
            cout << "BestDrivingRoute:none\n";
            return;
        }

        cout << "BestDrivingRoute:";
        for (size_t i = 0; i < bestPath.size(); ++i) {
            // Prints the shortest path
            cout << bestPath[i];
            if (i != bestPath.size() - 1) cout << ",";
        }
        cout << "(" << totalCost << ")\n";
    }
}

//same logic as planRestrictedRoute() but insatead of printing in the terminal, it prints in the output.txt
void planRestrictedRoute_(string source, string destination, set<int> avoidNodes, set<pair<int, int>> avoidSegments, string includeNode, ofstream &output) {
    output << "Source:" << source << "\nDestination: " << destination << "\n";
    // Convert source and destination to integers
    int src = stoi(source);
    int dst = stoi(destination);

    // Create a modified graph with the avoided nodes and segments removed
    Graph<int> restrictedGraph = createGraphs::createGraphFromCSV("LocationsTest.csv", "DistancesTest.csv");  // Assume using CSV files for graph

    // Remove nodes from restricted graph
    for (const auto& node : avoidNodes) {
        restrictedGraph.removeVertex(node); // Remove node by ID
    }

    // Remove segments from restrictedGraph
    for (const auto& seg : avoidSegments) {
        restrictedGraph.removeEdge(seg.first, seg.second); // Remove segments by node pair
    }

    if (!includeNode.empty()) {
        double totalCost1 = 0;
        // Run Dijkstra from source to includeNode
        vector<int> pathToIncludeNode = RoutePlanner::dijkstraShortestPath(restrictedGraph, src, stoi(includeNode), totalCost1);

        if (pathToIncludeNode.empty()) {
            output << "BestDrivingRoute:none\n";
            output << "AlternativeDrivingRoute:none\n";
            return;
        }

        // Run Dijkstra from includeNode to destination
        double totalCost2 = 0;
        vector<int> pathFromIncludeNode = RoutePlanner::dijkstraShortestPath(restrictedGraph, stoi(includeNode), dst, totalCost2);

        if (pathFromIncludeNode.empty()) {
            output << "BestDrivingRoute:none\n";
            output << "AlternativeDrivingRoute:none\n";
            return;
        }

        // Combine the two paths (source -> includeNode, includeNode -> destination)
        pathToIncludeNode.insert(pathToIncludeNode.end(), pathFromIncludeNode.begin() + 1, pathFromIncludeNode.end());

        
        output << "BestDrivingRoute:";
        for (size_t i = 0; i < pathToIncludeNode.size(); ++i) {
            // Prints the shortest path
            output << pathToIncludeNode[i];
            if (i != pathToIncludeNode.size() - 1) output << ",";
        }
        output << "(" << totalCost1 + totalCost2 << ")\n";
    } else {

    // Find the shortest path using Dijkstra’s algorithm on the restricted graph
    double totalCost = 0;
    vector<int> bestPath = RoutePlanner::dijkstraShortestPath(restrictedGraph, src, dst, totalCost);

    if (bestPath.empty()) {
        output << "BestDrivingRoute:none\n";
        return;
    }

    
    output << "BestDrivingRoute:";
    for (size_t i = 0; i < bestPath.size(); ++i) {
        // Prints the shortest path
        output << bestPath[i];
        if (i != bestPath.size() - 1) output << ",";
    }
    output << "(" << totalCost << ")\n";
    }
}

void planRouteWithMaxWalkTime(Graph<int>& g, int start, int end, int maxWalkTime, 
    const set<int>& avoidNodes, const set<pair<int, int>>& avoidSegments) {

    unordered_set<int> avoidNodesSet(avoidNodes.begin(), avoidNodes.end()); // Unordered set of the nodes to avoid
    unordered_set<pair<int, int>, pair_hash> avoidSegmentsSet(avoidSegments.begin(), avoidSegments.end()); // Unordered set of the segments to avoid

    auto [drivingRoute, parkingNode, walkingRoute, totalTime, message] = 
        RoutePlanner::findBestDriveWalkRoute(g, start, end, maxWalkTime, avoidNodesSet, avoidSegmentsSet); // Computes the best drive walk route

    cout << "Source:" << start << "\n";
    cout << "Destination:" << end << "\n";

    int routeCounter = 1;  // Initialize the route index counter

    if (drivingRoute.empty()) {
        cout << "DrivingRoute:\n";
        cout << "ParkingNode:\n";
        cout << "WalkingRoute:\n";
        cout << "TotalTime:\n";
        if (!message.empty()) {
            cout << "Message:" << message << "\n";
        }
        // If no valid route is found, try approximate solutions
        auto alternatives = RoutePlanner::findApproximateDriveWalkRoutes(g, start, end, maxWalkTime, avoidNodesSet, avoidSegmentsSet);

        // Output the alternatives in the desired format
        for (const auto& alt : alternatives) {
            const auto& [drivePath, parkingId, walkPath, totalTime] = alt;

            // Compute driving cost (time) for the driving route
            double driveCost = 0;
            for (size_t i = 0; i < drivePath.size() - 1; ++i) {
                Vertex<int>* v = g.findVertex(drivePath[i]);
                for (Edge<int>* e : v->getAdj()) {
                    if (e->getDest()->getInfo() == drivePath[i + 1]) {
                        driveCost += e->getDrivingWeight();
                        break;
                    }
                }
            }

            // Compute walking cost (time) for the walking route
            double walkCost = 0;
            for (size_t i = 0; i < walkPath.size() - 1; ++i) {
                Vertex<int>* v = g.findVertex(walkPath[i]);
                for (Edge<int>* e : v->getAdj()) {
                    if (e->getDest()->getInfo() == walkPath[i + 1]) {
                        walkCost += e->getWalkingWeight();
                        break;
                    }
                }
            }

            // Print the output in the desired format
            cout << "DrivingRoute" << routeCounter << ":";
            for (size_t i = 0; i < drivePath.size(); ++i) {
                cout << drivePath[i];
                if (i != drivePath.size() - 1) cout << ",";
            }
            cout << "(" << static_cast<int>(driveCost) << ")\n";  // Driving route cost

            cout << "ParkingNode" << routeCounter << ":" << parkingId << "\n";

            cout << "WalkingRoute" << routeCounter << ":";
            for (size_t i = 0; i < walkPath.size(); ++i) {
                cout << walkPath[i];
                if (i != walkPath.size() - 1) cout << ",";
            }
            cout << "(" << static_cast<int>(walkCost) << ")\n";  // Walking route cost

            cout << "TotalTime" << routeCounter << ":" << static_cast<int>(driveCost + walkCost) << "\n";

            routeCounter++;  // Increment the route index for the next alternative
        }

        return;  // Exit the function after showing alternatives
    }

    // Compute driving cost
    double driveCost = 0;
    for (size_t i = 0; i < drivingRoute.size() - 1; ++i) {
        Vertex<int>* v = g.findVertex(drivingRoute[i]);
        for (Edge<int>* e : v->getAdj()) {
            if (e->getDest()->getInfo() == drivingRoute[i + 1]) {
                driveCost += e->getDrivingWeight();
                break;
            }
        }
    }

    // --- Compute walking cost ---
    double walkCost = 0;
    for (size_t i = 0; i < walkingRoute.size() - 1; ++i) {
        Vertex<int>* v = g.findVertex(walkingRoute[i]);
        for (Edge<int>* e : v->getAdj()) {
            if (e->getDest()->getInfo() == walkingRoute[i + 1]) {
                walkCost += e->getWalkingWeight();
                break;
            }
        }
    }

    // --- Output: Driving Route ---
    cout << "DrivingRoute" << routeCounter << ":";
    for (size_t i = 0; i < drivingRoute.size(); ++i) {
        cout << drivingRoute[i];
        if (i != drivingRoute.size() - 1) cout << ",";
    }
    cout << "(" << static_cast<int>(driveCost) << ")\n";

    // --- Parking Node ---
    cout << "ParkingNode" << routeCounter << ":" << parkingNode << "\n";

    // --- Output: Walking Route ---
    cout << "WalkingRoute" << routeCounter << ":";
    for (size_t i = 0; i < walkingRoute.size(); ++i) {
        cout << walkingRoute[i];
        if (i != walkingRoute.size() - 1) cout << ",";
    }
    cout << "(" << static_cast<int>(walkCost) << ")\n";

    // --- Total Time ---
    cout << "TotalTime" << routeCounter << ":" << static_cast<int>(driveCost + walkCost) << "\n";
}



void planRouteWithMaxWalkTime_(Graph<int>& g, int start, int end, int maxWalkTime, 
    const set<int>& avoidNodes, const set<pair<int, int>>& avoidSegments, ofstream &output) {

    unordered_set<int> avoidNodesSet(avoidNodes.begin(), avoidNodes.end());
    unordered_set<pair<int, int>, pair_hash> avoidSegmentsSet(avoidSegments.begin(), avoidSegments.end());

    auto [drivingRoute, parkingNode, walkingRoute, totalTime, message] = 
        RoutePlanner::findBestDriveWalkRoute(g, start, end, maxWalkTime, avoidNodesSet, avoidSegmentsSet);

    output << "Source:" << start << "\n";
    output << "Destination:" << end << "\n";

    if (drivingRoute.empty()) {
        output << "DrivingRoute:none\n";
        output << "ParkingNode:none\n";
        output << "WalkingRoute:none\n";
        output << "TotalTime:none\n";
        if (!message.empty()) {
            output << "Message:" << message << "\n";
        }

        // If no valid route is found, try approximate solutions
        auto alternatives = RoutePlanner::findApproximateDriveWalkRoutes(g, start, end, maxWalkTime, avoidNodesSet, avoidSegmentsSet);

        int alternativeCount = 1;
        for (const auto& alt : alternatives) {
            const auto& [drivePath, parkingId, walkPath, totalTime] = alt;
            output << "DrivingRoute" << alternativeCount << ":";
            for (size_t i = 0; i < drivePath.size(); ++i) {
                output << drivePath[i];
                if (i != drivePath.size() - 1) output << ",";
            }
            output << "(" << static_cast<int>(totalTime) << ")\n";
            
            output << "ParkingNode" << alternativeCount << ":" << parkingId << "\n";

            output << "WalkingRoute" << alternativeCount << ":";
            for (size_t i = 0; i < walkPath.size(); ++i) {
                output << walkPath[i];
                if (i != walkPath.size() - 1) output << ",";
            }
            output << "(" << static_cast<int>(totalTime) << ")\n";

            output << "TotalTime" << alternativeCount << ":" << static_cast<int>(totalTime) << "\n";

            alternativeCount++;
            if (alternativeCount > 2) break;  // Only show two alternatives
        }

        return;  // Exit the function after showing alternatives
    }

    // --- Compute driving cost ---
    double driveCost = 0;
    output << "DrivingRoute1:";
    for (size_t i = 0; i < drivingRoute.size() - 1; ++i) {
        Vertex<int>* v = g.findVertex(drivingRoute[i]);
        for (Edge<int>* e : v->getAdj()) {
            if (e->getDest()->getInfo() == drivingRoute[i + 1]) {
                driveCost += e->getDrivingWeight();
                output << drivingRoute[i] << "(" << e->getDrivingWeight() << ")";
                if (i != drivingRoute.size() - 2) output << ",";
                break;
            }
        }
    }
    output << "(" << static_cast<int>(driveCost) << ")\n";

    // --- Parking Node ---
    output << "ParkingNode1:" << parkingNode << "\n";

    // --- Output: Walking Route ---
    output << "WalkingRoute1:";
    double walkCost = 0;
    for (size_t i = 0; i < walkingRoute.size() - 1; ++i) {
        Vertex<int>* v = g.findVertex(walkingRoute[i]);
        for (Edge<int>* e : v->getAdj()) {
            if (e->getDest()->getInfo() == walkingRoute[i + 1]) {
                walkCost += e->getWalkingWeight();
                output << walkingRoute[i] << "(" << e->getWalkingWeight() << ")";
                if (i != walkingRoute.size() - 2) output << ",";
                break;
            }
        }
    }
    output << "(" << static_cast<int>(walkCost) << ")\n";

    // --- Total Time ---
    output << "TotalTime1:" << static_cast<int>(driveCost + walkCost) << "\n";
}




void runBatchMode() {
    ifstream input("input.txt");
    ofstream output("output.txt");
    Graph<int> g = createGraphs::createGraphFromCSV("LocationsTest.csv", "DistancesTest.csv");

    if (!input.is_open() || !output.is_open()) {
        cerr << "Error: Could not open input or output file.\n";
        return;
    }

    string line, mode, source, destination, includeNode, MaxWalkTime;
    set<int> avoidNodes;
    set<pair<int, int>> avoidSegments;

    while (getline(input, line)) {
        if (line.find("Mode:") == 0) {
            mode = line.substr(5);
            getline(input, line);
            if (line.find("Source:") == 0) {
                source = line.substr(7);
                getline(input, line);
                if (line.find("Destination:") == 0) {
                    destination = line.substr(12);
                    getline(input, line);

                    if(line.find("MaxWalkTime:") == 0) {
                        MaxWalkTime = line.substr(12);
                        getline(input, line);
                    }

                    if (line.find("AvoidNodes:") == 0) {
                        stringstream ss(line.substr(11));
                        string node;
                        while (getline(ss, node, ',')) {
                            int newnode = stoi(node);
                            avoidNodes.insert(newnode);
                        }
                        getline(input, line);
                    }

                    if (line.find("AvoidSegments:") == 0) {
                        stringstream ss(line.substr(14)); 
                        string s = ss.str();
                        int x = 0;
                        while(s[x] != '\0') {
                            if(s[x] == '(') {
                                int first = stoi(string(1, s[x+1]));
                                int second = stoi(string(1, s[x+3]));
                                avoidSegments.insert({first, second});
                            }
                            x++;
                        }
                        getline(input, line);                
                    }

                    if (line.find("IncludeNode:") == 0) {
                        includeNode = line.substr(12);
                    }
                    if(avoidNodes.empty() && avoidSegments.empty() && includeNode.empty() && MaxWalkTime.empty()) planRoute_(g, source, destination, output);
                    else if(MaxWalkTime.empty()) planRestrictedRoute_(source, destination, avoidNodes, avoidSegments, includeNode, output);
                    else planRouteWithMaxWalkTime_(g, stoi(source), stoi(destination), stoi(MaxWalkTime), avoidNodes, avoidSegments, output);
                }
            }
        }
    }
    input.close();
    output.close();
}

int main(int argc, char *argv[]) {
    Graph<int> g = createGraphs::createGraphFromCSV("LocationsTest.csv", "DistancesTest.csv");
    if (argc > 1 && string(argv[1]) == "batch") {
        runBatchMode();
    } else {
        int choice;
        do {
            displayMenu();
            cin >> choice;

            string source, destination, includeNode, mode;
            set<string> avoidNodes;
            set<pair<string, string>> avoidSegments;
            int MaxWalkTime;

            switch (choice) {
                case 1: {
                    cout << "Mode: ";
                    cin >> mode;
                    cout << "Source: ";
                    cin >> source;
                    cout << "Destination: ";
                    cin >> destination;
                    planRoute(g,source, destination);
                    break;
                }
                case 2: {
                    cout << "Mode: ";
                    cin >> mode;
                    cout << "Source: ";
                    cin >> source;
                    cout << "Destination: ";
                    cin >> destination;
                
                    cout << "AvoidNodes: "; // ex: A,B,C,D
                    string nodes;
                    cin.ignore(); // Clear any leftover input from the buffer (useful if there are unexpected extra newlines).
                    getline(cin, nodes);  // Use getline to get full line input
                
                    if (!nodes.empty()) {
                        stringstream ss(nodes);
                        string node;
                        while (getline(ss, node, ',')) {
                            if (!node.empty()) {
                                avoidNodes.insert(node);
                            }
                        }
                    }
                
                    cout << "AvoidSegments: ";
                    string seg;
                    getline(cin, seg);  // Use getline to capture the full line for AvoidSegments
                
                    if (!seg.empty()) {
                        int x = 0;
                        while(seg[x] != '\0') {
                            if(seg[x] == '(') {
                                string first = string(1, seg[x+1]);
                                string second = string(1, seg[x+3]);
                                avoidSegments.insert({first, second});
                            }
                            x++;
                        }
                    }
                
                    cout << "IncludeNode: ";
                    getline(cin, includeNode);
                
                    planRestrictedRoute(source, destination, avoidNodes, avoidSegments, includeNode);
                    break;
                }
                             
                case 3: {
                    cout << "Mode: ";
                    cin >> mode;
                    cout << "Source: ";
                    cin >> source;
                    cout << "Destination: ";
                    cin >> destination;
                
                    cout << "MaxWalkTime: ";
                    cin >> MaxWalkTime;
                
                    set<int> avoidNodes;
                    cout << "AvoidNodes: ";
                    string nodes;
                    cin.ignore();
                    getline(cin, nodes);
                    if (!nodes.empty()) {
                        stringstream ss(nodes);
                        string node;
                        while (getline(ss, node, ',')) {
                            if (!node.empty()) {
                                avoidNodes.insert(stoi(node)); // assumes IDs are numeric
                            }
                        }
                    }
                
                    cout << "AvoidSegments: ";
                    set<pair<int, int>> avoidSegments;
                    string seg;
                    getline(cin, seg);
                    if (!seg.empty()) {
                        stringstream ss(seg);
                        char c;
                        while (ss >> c) {
                            if (c == '(') {
                                int from, to;
                                ss >> from;
                                ss.ignore(); // skip comma
                                ss >> to;
                                ss.ignore(); // skip closing )
                                avoidSegments.insert({from, to});
                            }
                        }
                    }
                
                    planRouteWithMaxWalkTime(g, stoi(source), stoi(destination), MaxWalkTime, avoidNodes, avoidSegments);
                    break;
                }              
                case 0: {
                    cout << "Exiting...\n";
                    break;
                }
                default:
                    cout << "Invalid choice! Try again.\n";
            }
        } while (choice != 0);
    }
    return 0;
}