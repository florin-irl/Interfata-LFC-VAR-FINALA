#ifndef NODE_H
#define NODE_H

#include <QPainter>

class Node
{
    int value;
    QPoint coordinate;
    bool selected;

public:

    Node();
    Node(QPoint p);
    void setCoordinate(QPoint p);
    void setValue(int v);
    QPoint getCoordinate() const;
    int getValue();
    int getX();
    int getY();
};

#endif // NODE_H
