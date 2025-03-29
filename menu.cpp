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
    cout << "====== Route Planning Tool ======\n";
    cout << "1. Independent Route Planning\n";
    cout << "2. Restricted Route Planning\n";
    cout << "3. Environmentally-Friendly Route Planning (driving and walking)\n";
    cout << "0. Exit\n";
    cout << "Enter your choice: ";
}

void planRoute(Graph<int>& g, string start, string end) {
    cout << "Planning route from " << start << " to " << end << "...\n";
    int src = stoi(start);
    int dst = stoi(end);

    double totalCost = 0;
    
    vector<int> bestPath = RoutePlanner::dijkstraShortestPath(g, src, dst, totalCost);

    if (bestPath.empty()) {
        cout << "BestDrivingRoute:none\n";
        cout << "AlternativeDrivingRoute:none\n";
        return;
    }

    cout << "BestDrivingRoute:";
    for (size_t i = 0; i < bestPath.size(); ++i) {
        cout << bestPath[i];
        if (i != bestPath.size() - 1) cout << ",";
    }
    cout << "(" << totalCost << ")\n";

    double totalCost2 = 0;

    vector<int> altPath = RoutePlanner::findAlternativePath(g, src, dst, bestPath, totalCost2);

    if (altPath.empty()) {
        cout << "AlternativeDrivingRoute:none\n";
        return;
    }


    cout << "AlternativeDrivingRoute:";
    for (size_t i = 0; i < altPath.size(); ++i) {
        cout << altPath[i];
        if (i != altPath.size() - 1) cout << ",";
    }
    cout << "(" << totalCost2 << ")\n";
}

void planRoute_(string start, string end) {

    cout << "Planning route from " << start << " to " << end << "...\n";
}

void planRouteWithExclusions(string start, string end, set<string> avoidNodes, set<pair<char, char>> avoidSegments, string includeNode) {
    cout << "Planning route from " << start << " to " << end << " with exclusions...\n";
    cout << "Avoiding Nodes: ";
    for (const auto& node : avoidNodes) cout << node << " ";
    cout << "\nAvoiding Segments: ";
    for (const auto& seg : avoidSegments) cout << "(" << seg.first << "," << seg.second << ") ";
    cout << "\nIncluding Node: " << includeNode << endl;

    cout << "BestDrivingRoute: " << start << " -> " << end << " (Time: X)\n";
}

void planRouteWithExclusions_(string start, string end, set<string> avoidNodes, set<pair<char, char>> avoidSegments, string includeNode, ofstream &output) {
    // Writing to output file
    output << "Planning route from " << start << " to " << end << " with exclusions...\n";
    output << "Avoiding Nodes: ";
    for (const auto& node : avoidNodes) output << node << " ";
    output << "\nAvoiding Segments: ";
    for (const auto& seg : avoidSegments) output << "(" << seg.first << "," << seg.second << ") ";
    output << "\nIncluding Node: " << includeNode << "\n";

    output << "BestDrivingRoute: " << start << " -> " << end << " (Time: X)\n"; // Placeholder
}

void planRouteWithMaxWalkTime(string start, string end, int MaxWalkTime, set<string> avoidNodes, set<pair<char,char>> avoidSegments) {
    cout << "Planning route from " << start << " to " << end << " with exclusions...\n";
    cout << "MaxWalkTime: " << MaxWalkTime << "\n";
    cout << "Avoiding Nodes: ";
    for (const auto& node : avoidNodes) cout << node << " ";
    cout << "\nAvoiding Segments: ";
    for (const auto& seg : avoidSegments) cout << "(" << seg.first << "," << seg.second << ") ";
    cout << "\nBestDrivingRoute: " << start << " -> " << end << " (Time: X)\n";
}

void planRouteWithMaxWalkTime_(string start, string end, string MaxWalkTime, set<string> avoidNodes, set<pair<char,char>> avoidSegments, ofstream &output) {
    output << "Planning route from " << start << " to " << end << " with exclusions...\n";
    output << "MaxWalkTime: " << MaxWalkTime << "\n";
    output << "Avoiding Nodes: ";
    for (const auto& node : avoidNodes) output << node << " ";
    output << "\nAvoiding Segments: ";
    for (const auto& seg : avoidSegments) output << "(" << seg.first << "," << seg.second << ") ";

    output << "\nBestDrivingRoute: " << start << " -> " << end << " (Time: X)\n"; // Placeholder
}

void runBatchMode() {
    ifstream input("input.txt");
    ofstream output("output.txt");

    if (!input.is_open() || !output.is_open()) {
        cerr << "Error: Could not open input or output file.\n";
        return;
    }

    string line, mode, source, destination, includeNode, MaxWalkTime;
    set<string> avoidNodes;
    set<pair<char, char>> avoidSegments;

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
                            avoidNodes.insert(node);
                        }
                        getline(input, line);
                    }

                    if (line.find("AvoidSegments:") == 0) {
                        stringstream ss(line.substr(14)); 
                        string s = ss.str();
                        int x = 0;
                        while(s[x] != '\0') {
                            if(s[x] == '(') {
                                avoidSegments.insert({s[x+1], s[x+3]});
                            }
                            x++;
                        }
                        getline(input, line);                
                    }

                    if (line.find("IncludeNode:") == 0) {
                        includeNode = line.substr(12);
                    }

                    if (avoidNodes.empty() && avoidSegments.empty() && MaxWalkTime.empty() && includeNode.empty()) {
                        //planRoute(source, destination, output);
                    } else if(MaxWalkTime.empty()) {
                        planRouteWithExclusions_(source, destination, avoidNodes, avoidSegments, includeNode, output);
                    }
                    else planRouteWithMaxWalkTime_(source, destination, MaxWalkTime, avoidNodes, avoidSegments, output);
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
            set<pair<char, char>> avoidSegments;
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
                    } else {
                        cout << "No nodes to avoid.\n";  // Optional: Show that no nodes are being avoided
                    }

                    cout << "AvoidSegments: "; // ex: (1,2),(2,3),(3,4)
                    set<pair<char, char>> avoidSegments_;
                    string seg;
                    getline(cin, seg);  // Use getline to capture the full line for AvoidSegments

                    if (!seg.empty()) {
                        stringstream ss(seg);
                        char first, second;
                        while (ss >> first) {
                            if (first == '(') {
                                ss >> second;  // Get the first character of the pair
                                ss.ignore();   // Ignore the comma between the characters
                                char third;
                                ss >> third;   // Get the second character of the pair
                                ss.ignore();   // Ignore the closing parenthesis
                                avoidSegments_.insert({second, third});  // Insert the segment pair
                            }
                        }
                    } else {
                        cout << "No segments to avoid.\n";  // Optional: Show that no segments are being avoided
                    }

                    cout << "IncludeNode: ";
                    cin >> includeNode;
                
                    planRouteWithExclusions(source, destination, avoidNodes, avoidSegments_, includeNode);  // Pass avoidSegments correctly
                    break;
                }                
                case 3: {
                    cout << "Mode: ";
                    cin >> mode;
                    cout << "Source: ";
                    cin >> source;
                    cout << "Destination: ";
                    cin >> destination;
                
                    cout << "MaxWalkTime: "; // ex: 18
                    cin >> MaxWalkTime;
                
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
                    } else {
                        cout << "No nodes to avoid.\n";  // Optional: Show that no nodes are being avoided
                    }

                
                    cout << "AvoidSegments: "; // ex: (1,2),(2,3),(3,4)
                    set<pair<char, char>> avoidSegments_;
                    string seg;
                    getline(cin, seg);  // Use getline to capture the full line for AvoidSegments

                    if (!seg.empty()) {
                        stringstream ss(seg);
                        char first, second;
                        while (ss >> first) {
                            if (first == '(') {
                                ss >> second;  // Get the first character of the pair
                                ss.ignore();   // Ignore the comma between the characters
                                char third;
                                ss >> third;   // Get the second character of the pair
                                ss.ignore();   // Ignore the closing parenthesis
                                avoidSegments_.insert({second, third});  // Insert the segment pair
                            }
                        }
                    } else {
                        cout << "No segments to avoid.\n";  // Optional: Show that no segments are being avoided
                    }

                    planRouteWithMaxWalkTime(source, destination, MaxWalkTime, avoidNodes, avoidSegments_);
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