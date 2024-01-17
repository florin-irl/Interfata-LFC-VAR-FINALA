#include "node.h"

Node::Node()
{
    value = -1;
    selected = false;
}

Node::Node(QPoint p)
{
    coordinate = p;
}

int Node::getValue()
{
    return value;
}

void Node::setValue(int v)
{
    value = v;
}

void Node::setCoordinate(QPoint p)
{
    coordinate = p;
}

QPoint Node::getCoordinate() const
{
    return coordinate;
}

int Node::getX()
{
    return coordinate.x();
}

int Node::getY()
{
    return coordinate.y();
}

