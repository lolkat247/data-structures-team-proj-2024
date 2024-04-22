//
// Created by Jake Iglesias on 4/20/24.
//

#include "Graph.h"
#include <iostream>
#include <queue>
#include <limits>
#include <unordered_map>
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

Graph::PathResult Graph::findPath(const string& startCode, const string& goalCode, bool useCost) {
    PathResult result;
    if (airports.find(startCode) == airports.end() || airports.find(goalCode) == airports.end()) {
        cout << "One or both airports not found." << endl;
        return result;
    }

    auto comp = [&](const pair<int, string>& left, const pair<int, string>& right) {
        return left.first > right.first;
    };
    priority_queue<pair<int, string>, vector<pair<int, string>>, decltype(comp)> pq(comp);

    for (const auto& airport : airports) {
        result.costs[airport.first] = numeric_limits<int>::max();
    }

    result.costs[startCode] = 0;
    pq.push({0, startCode});

    while (!pq.empty()) {
        auto [currentCost, currentCode] = pq.top();
        pq.pop();

        if (currentCode == goalCode) {
            break;
        }

        for (const auto& edge : airports[currentCode]->connections) {
            string neighborCode = edge.destination->code;
            int weight = useCost ? edge.cost : edge.distance;

            if (currentCost + weight < result.costs[neighborCode]) {
                result.costs[neighborCode] = currentCost + weight;
                result.predecessors[neighborCode] = currentCode;
                pq.push({result.costs[neighborCode], neighborCode});
            }
        }
    }

    return result;
}

vector<string> Graph::findShortestPath(const string& startCode, const string& goalCode, int& pathLength) {
    PathResult result = findPath(startCode, goalCode, false);  // false for distance
    if (result.predecessors.find(goalCode) == result.predecessors.end()) {
//        cout << "No path exists." << endl;
        return {};
    }

    vector<string> path;
    for (string at = goalCode; at != startCode; at = result.predecessors[at]) {
        path.push_back(at);
    }
    path.push_back(startCode);
    reverse(path.begin(), path.end());
    pathLength = result.costs[goalCode];

    return path;
}

vector<string> Graph::findLeastExpensivePath(const string& startCode, const string& goalCode, int& totalCost) {
    PathResult result = findPath(startCode, goalCode, true);  // true for cost
    if (result.predecessors.find(goalCode) == result.predecessors.end()) {
//        cout << "No path exists." << endl;
        return {};
    }

    vector<string> path;
    for (string at = goalCode; at != startCode; at = result.predecessors[at]) {
        path.push_back(at);
    }
    path.push_back(startCode);
    reverse(path.begin(), path.end());
    totalCost = result.costs[goalCode];

    return path;
}

unordered_map<string, Airport*> Graph::getAirports() {
    return airports;
}

vector<Airport*> Graph::getAllAirportsInState(string stateCode) {
    vector<Airport*> airportsInState;
    for (auto& pair : airports) {
        if (pair.second->getState() == stateCode) airportsInState.push_back(pair.second);
    }

    return airportsInState;
}