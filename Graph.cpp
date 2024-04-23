//
// Created by Jake Iglesias on 4/20/24.
//

#include "Graph.h"
#include "UnionFind.h"
#include <iostream>
#include <queue>
#include <climits>
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

        pair<int, string> topElement = pq.top();
        int currentCost = topElement.first;
        string currentCode = topElement.second;
        pq.top();
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

// dijkstra
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

void Graph::countAndDisplayFlightConnections() {
    unordered_map<string, int> flightConnections;
    for (const auto& pair : airports) {
        const string& airportCode = pair.first;
        const Airport* airport = pair.second;

        // Count inbound flights
        int inboundCount = 0;
        for (const auto& otherPair : airports) {
            if (otherPair.first != airportCode) {
                for (const auto& edge : otherPair.second->connections) {
                    if (edge.destination->code == airportCode) {
                        ++inboundCount;
                        break;
                    }
                }
            }
        }
        // Count outbound flights
        int outboundCount = airport->connections.size();

        // Total flight connections
        int totalConnections = inboundCount + outboundCount;

        // Store connections for airport
        flightConnections[airportCode] = totalConnections;
    }

    // Sort the airports (descending) by total connections
    vector<pair<string, int>> sortedConnections(
            flightConnections.begin(), flightConnections.end());
    sort(sortedConnections.begin(), sortedConnections.end(), compareConnections);

    // Display sorted list
    cout << "Total Direct Flight Connections:\n";
    for (const auto& pair : sortedConnections) {
        cout << "Airport: " << pair.first << ", Total Connections: " << pair.second << "\n";
    }
}

// Comparing connections function
bool Graph::compareConnections(const pair<string, int>& a, const pair<string, int>& b) {
    return a.second > b.second;
}

Graph Graph::createUndirectedGraph() {
    Graph undirectedGraph;
    // Current airport...
    for (const auto& pair : airports) {
        const string& originCode = pair.first;
        const Airport* originAirport = pair.second;

        //...Goes through airport connected to current airport
        for (const auto& edge : originAirport->connections) {
            const string& destinationCode = edge.destination->code;
            int cost = edge.cost;

            // Check for opposite edge
            bool hasOppositeEdge = false;
            for (const auto& oppositeEdge : edge.destination->connections) {
                if (oppositeEdge.destination->code == originCode) {
                    hasOppositeEdge = true;
                    // Keeps edge with smaller cost
                    if (oppositeEdge.cost < cost) {
                        cost = oppositeEdge.cost;
                    }
                    break;
                }
            }

            // Add the edge to undirected graph
            undirectedGraph.addFlight(originCode, destinationCode, "", "", 0, cost);
        }
    }

    return undirectedGraph;
}

// modified bfs
vector<string> Graph::findShortestPathWithStops(const string& startCode, const string& goalCode,
                                                int& pathLength, int& numberOfStops) {
    if (airports.find(startCode) == airports.end() || airports.find(goalCode) == airports.end()) {
        cout << "One or both airports not found." << endl; // shouldn't happen
        return {};
    }

    struct Node {
        string code;
        int totalDistance;
        int stops;
        vector<string> path;
    };

    queue<Node> q;
    q.push({startCode, 0, 0, {startCode}});

    int minPathLength = numeric_limits<int>::max();
    vector<string> shortestPath;

    while (!q.empty()) {
        Node current = q.front();
        q.pop();

        // check if node reaches with the exact number of stops
        if (current.code == goalCode && current.stops == numberOfStops) {
            if (current.totalDistance < minPathLength) {
                minPathLength = current.totalDistance;
                shortestPath = current.path;
            }
            continue;
        }

        // continue if not
        if (current.stops < numberOfStops) {
            for (const auto& edge : airports[current.code]->connections) {
                if (find(current.path.begin(), current.path.end(),
                         edge.destination->code) == current.path.end()) { // Avoid cycles
                    vector<string> newPath = current.path;
                    newPath.push_back(edge.destination->code);
                    q.push({edge.destination->code, current.totalDistance + edge.distance,
                            current.stops + 1, newPath});
                }
            }
        }
    }

    pathLength = minPathLength;
    return shortestPath;
}
Graph Graph:: Prims(const string & startCode)
{
    Graph MST;
    using Edge = pair<int, string>;

    priority_queue<Edge, vector<Edge>, greater<Edge>> minHeap;
    unordered_map<string, int> key;
    unordered_map<string, string> parent;
    unordered_map<string, bool> inMST;


    for (const auto& pair : airports) {
        key[pair.first] = INT_MAX;
        parent[pair.first] = "";
        inMST[pair.first] = false;
    }


    key[startCode] = 0;
    minHeap.push({0, startCode});

    while (!minHeap.empty()) {
        string u = minHeap.top().second;
        minHeap.pop();


        inMST[u] = true;


        for (const auto& connection : airports[u]->connections) {
            string v = connection.destination->code;
            int weight = connection.cost;


            if (!inMST[v] && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
                minHeap.push({weight, v});
            }
        }
    }


    for (const auto& pair : parent) {
        if (pair.second != "") {
            MST.addFlight(pair.second, pair.first, "", "", 0, key[pair.first]);
        }
    }

    return MST;
}
Graph Graph::Kruskal()
{
    Graph mst;  // The resulting MST

    // Collect all edges from the graph
    std::vector<std::pair<int, std::pair<std::string, std::string>>> edges;
    for (const auto& airport : airports) {
        const std::string& origin = airport.first;
        for (const auto& connection : airport.second->connections) {
            const std::string& destination = connection.destination->code;
            int cost = connection.cost;
            edges.emplace_back(cost, std::make_pair(origin, destination));
        }
    }

    // Sort the edges by weight
    std::sort(edges.begin(), edges.end());

    // Initialize the union-find structure
    UnionFind uf;
    for (const auto& airport : airports) {
        uf.makeSet(airport.first);
    }

    // Iterate over the sorted edges to build the MST
    for (const auto& edge : edges) {
        int cost = edge.first;
        const std::string& origin = edge.second.first;
        const std::string& destination = edge.second.second;

        // Check if adding this edge creates a cycle
        if (uf.find(origin) != uf.find(destination)) {
            mst.addFlight(origin, destination, "", "", 0, cost);  // Add edge to MST
            uf.unionSets(origin, destination);  // Join the sets to prevent future cycles
        }
    }

    return mst;  // Return the constructed MST

}
void Graph:: displayMST() const {
    int totalCost;
    cout << "Minimal Spanning Tree:\n";
    for (const auto& pair : airports) {
        const string& origin = pair.first;
        for (const auto& connection : pair.second->connections) {
            const string& destination = connection.destination->code;
            int cost = connection.cost;
            cout << "Edge: (" << origin << "-> " << destination << ") - Cost: " << cost << "\n";

            totalCost=totalCost+cost;

        }
    }
    cout<<"Total Cost:           "<<totalCost<<endl;
}