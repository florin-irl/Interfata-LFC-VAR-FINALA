#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_set>
#include <stack>
#include "edge.h"

class Graph
{
    std::vector<Edge*> edges;
    std::vector<Node*> nodes;
    std::vector<std::vector<std::pair<Node*, float>>> adjacencyList;

public:
    Graph();
    ~Graph();
    void addNode(Node* n);
    void addNode(QPoint p);
    void addEdge(Node* n1, Node* n2, char cost);
    std::vector<Node*> getNodes();
    std::vector<Edge*> getEdges();
    Edge* findEdge(Node* n1, Node* n2);

    std::vector<std::pair<Node*, float>> getAdjacencyList(Node* node);

};
#endif // GRAPH_H
