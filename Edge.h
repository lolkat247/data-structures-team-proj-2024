//
// Created by Jake Iglesias on 4/20/24.
//

#ifndef DATA_STRUCTURES_TEAM_PROJ_2024_EDGE_H
#define DATA_STRUCTURES_TEAM_PROJ_2024_EDGE_H

class Airport; // Forward declaration

class Edge {
public:
    Airport* destination;
    int distance;
    int cost;

    Edge(Airport* dest, int dist, int cost) {
        this->destination = dest;
        this->distance = dist;
        this->cost = cost;
    }
};

#endif //DATA_STRUCTURES_TEAM_PROJ_2024_EDGE_H
