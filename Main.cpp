#include "Graph.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

vector<string> parseCSVLine(const string& line) {
    vector<string> fields;
    stringstream ss(line);
    string field;
    bool inQuotes = false;
    char ch;

    while (ss.get(ch)) {
        if (ch == '"') {
            inQuotes = !inQuotes;
        } else if (ch == ',' && !inQuotes) {
            fields.push_back(field);
            field.clear();
        } else {
            field += ch;
        }
    }
    fields.push_back(field); // Add the last field

    return fields;
}

void loadGraphData(Graph& graph, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }

    string line;
    int distance, cost;

    // Skip CSV header
    getline(file, line);

    while (getline(file, line)) {
        vector<string> fields = parseCSVLine(line);
        if (fields.size() < 6) {
            cerr << "Error parsing line: " << line << endl;
            continue;
        }

        string& originCode = fields[0];
        string& destinationCode = fields[1];
        string& originCityState = fields[2];
        string& destinationCityState = fields[3];
        string& distStr = fields[4];
        string& costStr = fields[5];

        try {
            distance = stoi(distStr);
            cost = stoi(costStr);
        } catch (const exception& e) {
            cerr << "Error converting distance or cost from line: " << line << endl;
            continue;
        }

        graph.addFlight(originCode, destinationCode, originCityState, destinationCityState, distance, cost);
        cout << "Added flight from " << originCode << " to " <<
            destinationCode << " with distance " << distance <<
            " and cost " << cost << endl;
    }
}

int main() {
    Graph graph;
    loadGraphData(graph, "airports.csv");
    cout << "Displaying graph:" << endl;
    graph.display();

    string startCode, endCode;
    cout << "Enter origin airport code: ";
    cin >> startCode;
    cout << "Enter destination airport code: ";
    cin >> endCode;

    // Calculate the shortest path based on distance
    int pathLength = 0;
    vector<string> path = graph.findShortestPath(startCode, endCode, pathLength);

    if (!path.empty()) {
        cout << "Shortest Path by distance: ";
        for (const string& code : path) {
            cout << code << " ";
        }
        cout << "\nLength of Shortest Path: " << pathLength << " km\n";
    } else {
        cout << "No path found or one of the airports does not exist based on distance." << endl;
    }

    // Calculate the least expensive path based on cost
    int totalCost = 0;
    vector<string> costPath = graph.findLeastExpensivePath(startCode, endCode, totalCost);

    if (!costPath.empty()) {
        cout << "Least Expensive Path by cost: ";
        for (const string& code : costPath) {
            cout << code << " ";
        }
        cout << "\nTotal Cost of Least Expensive Path: " << totalCost << " units\n";
    } else {
        cout << "No path found or one of the airports does not exist based on cost." << endl;
    }

    return 0;
}