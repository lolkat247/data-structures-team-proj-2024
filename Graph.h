//
// Created by Jake Iglesias on 4/20/24.
//

#ifndef DATA_STRUCTURES_TEAM_PROJ_2024_GRAPH_H

#include <map>
#include <string>
#include <vector>
#include <limits>
#include "Airport.h"

using namespace std;

class Graph {
private:
    map<string, Airport*> airports;

public:
    Graph();
    ~Graph();
    void addFlight(const string& originCode, const string& destinationCode,
                   const string& originCityState, const string& destinationCityState,
                   int distance, int cost);
    void display();
    vector<string> findShortestPath(const string& startCode, const string& goalCode, int& pathLength);
};

#define DATA_STRUCTURES_TEAM_PROJ_2024_GRAPH_H

#endif //DATA_STRUCTURES_TEAM_PROJ_2024_GRAPH_H
