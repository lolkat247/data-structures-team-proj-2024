//
// Created by Jake Iglesias on 4/20/24.
//

#ifndef DATA_STRUCTURES_TEAM_PROJ_2024_AIRPORT_H
#define DATA_STRUCTURES_TEAM_PROJ_2024_AIRPORT_H

#include <string>
#include <vector>
#include "Edge.h"

class Airport {
public:
    std::string code;
    std::vector<Edge> connections;

    Airport(const std::string& c) {
        this->code = c;
    }
};

#endif //DATA_STRUCTURES_TEAM_PROJ_2024_AIRPORT_H
