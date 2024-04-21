//
// Created by Jake Iglesias on 4/20/24.
//

#include "Graph.h"
#include <iostream>

using namespace std;

Graph::Graph() {}

Graph::~Graph() {
    for (auto& pair : airports) {
        delete pair.second;
    }
}

void Graph::addFlight(const string& origin, const string& destination, int distance, int cost) {
    if (airports.find(origin) == airports.end()) {
        airports[origin] = new Airport(origin);
    }
    if (airports.find(destination) == airports.end()) {
        airports[destination] = new Airport(destination);
    }
    airports[origin]->connections.emplace_back(airports[destination], distance, cost);
}

void Graph::display() {
    for (const auto& pair : airports) {
        cout << "Airport: " << pair.first << endl;
        for (const auto& edge : pair.second->connections) {
            cout << "  -> " << edge.destination->code << " (Distance: " << edge.distance << ", Cost: " << edge.cost << ")" << endl;
        }
    }
}