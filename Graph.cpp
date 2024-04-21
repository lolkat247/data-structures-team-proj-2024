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

void Graph::addFlight(const string& originCode, const string& destinationCode,
                      const string& originCityState, const string& destinationCityState,
                      int distance, int cost) {
    if (airports.find(originCode) == airports.end()) {
        airports[originCode] = new Airport(originCode, originCityState);
    }
    if (airports.find(destinationCode) == airports.end()) {
        airports[destinationCode] = new Airport(destinationCode, destinationCityState);
    }
    Airport* originAirport = airports[originCode];
    Airport* destinationAirport = airports[destinationCode];
    originAirport->connections.emplace_back(destinationAirport, distance, cost);
}

void Graph::display() {
    for (const auto& pair : airports) {
        cout << "Airport: " << pair.first << endl;
        for (const auto& edge : pair.second->connections) {
            cout << "  -> " << edge.destination->code << " (Distance: " << edge.distance << ", Cost: " << edge.cost << ")" << endl;
        }
    }
}