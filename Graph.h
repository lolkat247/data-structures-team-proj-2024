//
// Created by Jake Iglesias on 4/20/24.
//

#ifndef DATA_STRUCTURES_TEAM_PROJ_2024_GRAPH_H

#include <unordered_map>
#include <string>
#include <vector>
#include <limits>
#include "Airport.h"

using namespace std;

class Graph {
private:
    unordered_map<string, Airport*> airports;

    struct PathResult {
        unordered_map<string, int> costs;
        unordered_map<string, string> predecessors;
    };

    PathResult findPath(const string& startCode, const string& goalCode, bool useCost);

public:
    Graph();
    ~Graph();
    void addFlight(const string& originCode, const string& destinationCode,
                   const string& originCityState, const string& destinationCityState,
                   int distance, int cost);
    void display();
    vector<string> findShortestPath(const string& startCode, const string& goalCode, int& pathLength);
    vector<string> findLeastExpensivePath(const string& startCode, const string& goalCode, int& totalCost);
    unordered_map<string, Airport*> getAirports();
    vector<Airport*> getAllAirportsInState(string stateCode);
    void countAndDisplayFlightConnections();
    Graph createUndirectedGraph();
};

#define DATA_STRUCTURES_TEAM_PROJ_2024_GRAPH_H

#endif //DATA_STRUCTURES_TEAM_PROJ_2024_GRAPH_H
