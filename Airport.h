//
// Created by Jake Iglesias on 4/20/24.
//

#ifndef DATA_STRUCTURES_TEAM_PROJ_2024_AIRPORT_H
#define DATA_STRUCTURES_TEAM_PROJ_2024_AIRPORT_H

#include <string>
#include <vector>
#include "Edge.h"

using namespace std;

class Airport {
public:
    string code;
    string cityState;
    vector<Edge> connections;

    Airport(const string& code, const string& cityState);

    string getState() const;
};

#endif //DATA_STRUCTURES_TEAM_PROJ_2024_AIRPORT_H