//
// Created by Jake Iglesias on 4/20/24.
//

#include "Graph.h"
#include <iostream>
#include <queue>
#include <limits>
#include <map>
#include <algorithm>

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
            cout << "  -> " << edge.destination->code << " (Distance: " <<
                edge.distance << ", Cost: " << edge.cost << ")" << endl;
        }
    }
}

vector<string> Graph::findShortestPath(const string& startCode, const string& goalCode, int& pathLength) {
    if (airports.find(startCode) == airports.end() || airports.find(goalCode) == airports.end()) {
        cout << "One or both airports not found." << endl;
        return {};
    }

    unordered_map<string, int> distances;
    unordered_map<string, string> predecessors;
    auto comp = [&](const pair<int, string>& left, const pair<int, string>& right) {
        return left.first > right.first;
    };
    priority_queue<pair<int, string>, vector<pair<int, string>>, decltype(comp)> pq(comp);

    for (const auto& airport : airports) {
        distances[airport.first] = numeric_limits<int>::max();
    }

    distances[startCode] = 0;
    pq.push({0, startCode});

    while (!pq.empty()) {
        auto [currentDistance, currentCode] = pq.top();
        pq.pop();

        if (currentCode == goalCode) {
            pathLength = currentDistance;
            break;
        }

        for (const auto& edge : airports[currentCode]->connections) {
            string neighborCode = edge.destination->code;
            int weight = edge.distance;

            if (currentDistance + weight < distances[neighborCode]) {
                distances[neighborCode] = currentDistance + weight;
                predecessors[neighborCode] = currentCode;
                pq.push({distances[neighborCode], neighborCode});
            }
        }
    }

    if (predecessors.find(goalCode) == predecessors.end()) {
        cout << "No path exists." << endl;
        return {};
    }

    vector<string> path;
    for (string at = goalCode; at != startCode; at = predecessors[at]) {
        path.push_back(at);
    }
    path.push_back(startCode);
    reverse(path.begin(), path.end());

    return path;
}