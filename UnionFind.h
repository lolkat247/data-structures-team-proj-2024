#ifndef UNION_FIND_H
#define UNION_FIND_H

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>

using namespace std;

class UnionFind {
private:
    unordered_map<string, string> parent;
    unordered_map<string, int> rank;

public:
    void makeSet(const string& node) {
        parent[node] = node;  // Initially, each node is its own parent
        rank[node] = 0;       // Initially, each node has a rank of 0
    }

    string find(const string& node) {
        if (parent[node] != node) {
            parent[node] = find(parent[node]);  // Path compression
        }
        return parent[node];
    }

    void unionSets(const string& node1, const string& node2) {
        string root1 = find(node1);
        string root2 = find(node2);

        if (root1 != root2) {
            if (rank[root1] > rank[root2]) {
                parent[root2] = root1;
            } else if (rank[root1] < rank[root2]) {
                parent[root1] = root2;
            } else {
                parent[root2] = root1;
                rank[root1]++;
            }
        }
    }
};
#endif