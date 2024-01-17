#ifndef EDGE_H
#define EDGE_H

#include "node.h"

class Edge
{
    Node* firstNode, * secondNode;
    char m_cost;
public:
    Edge();
    Edge(Node* n1, Node* n2);
    Edge(Node* n1, Node* n2, char cost);
    Node* getFirstNode();
    Node* getSecondNode();
    void setFirstNode(Node* n);
    void setSecondNode(Node* n);
    char getCost();
    void setCost(float cost);
};

#endif // EDGE_H
