#include "edge.h"

Edge::Edge()
{
    firstNode = new Node;
    secondNode = new Node;
}

Edge::Edge(Node* n1, Node* n2)
    :firstNode{ n1 }
    , secondNode{ n2 }
{}

Edge::Edge(Node* n1, Node* n2, char cost)
    :firstNode{ n1 }
    , secondNode{ n2 }
    , m_cost{ cost }
{}

Node* Edge::getFirstNode()
{
    return firstNode;
}


Node* Edge::getSecondNode()
{
    return secondNode;
}

void Edge::setFirstNode(Node* n)
{
    firstNode = n;
}

void Edge::setSecondNode(Node* n)
{
    secondNode = n;
}

char Edge::getCost()
{
    return m_cost;
}

void Edge::setCost(float cost)
{
    m_cost = cost;
}
