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
    return 0;
}