#include "graph.h"
#include <fstream>
#include <stack>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <algorithm>

Graph::Graph()
{
}

void Graph::addNode(QPoint p)
{
    Node* n = new Node(p);
    n->setValue(nodes.size());
    nodes.push_back(n);
}

void Graph::addNode(Node* n)
{
    nodes.push_back(n);
}

void Graph::addEdge(Node* n1, Node* n2, char cost)
{
    Edge* edge = new Edge(n1, n2);
    edge->setCost(cost);
    edges.push_back(edge);

    int n1Value = n1->getValue();
    int n2Value = n2->getValue();


}

std::vector<Node*> Graph::getNodes()
{
    return nodes;
}

std::vector<Edge*> Graph::getEdges()
{
    return edges;
}

Graph::~Graph()
{
    for (auto& nod : nodes)
        delete nod;
    nodes.clear();

    for (auto& ed : edges)
        delete ed;
    edges.clear();
}

Edge* Graph::findEdge(Node* n1, Node* n2)
{
    for (const auto ed : edges)
        if ((ed->getFirstNode() == n1 && ed->getSecondNode() == n2) ||
            (ed->getFirstNode() == n2 && ed->getSecondNode() == n1))
            return ed;
    return nullptr;
}

std::vector<std::pair<Node*, float>> Graph::getAdjacencyList(Node* node)
{
    return adjacencyList[node->getValue() - 1];
}
