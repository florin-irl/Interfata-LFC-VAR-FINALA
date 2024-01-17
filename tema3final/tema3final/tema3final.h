#pragma once

#include <QtWidgets/QMainWindow>
#include <fstream>
#include "ui_tema3final.h"
#include "AFN.h"
#include "AFD.h"
#include "graph.h"

class tema3final : public QMainWindow
{
    Q_OBJECT

public:
    tema3final(QWidget* parent = nullptr);
    ~tema3final();

    FiniteAutomaton AFN;
    DeterministicFiniteAutomaton AFD;

    virtual void mouseReleaseEvent(QMouseEvent* e);
    virtual void paintEvent(QPaintEvent* e);
    virtual void mouseMoveEvent(QMouseEvent* e);
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void keyPressEvent(QKeyEvent* event);


private slots:
    void onCheckBoxStateChanged(int state);

private slots:
    void onPushButtonClicked();
    void onPushButton2Clicked();
    void onPushButton3Clicked();

private:
    std::set<std::pair<int, int>> addedEdges;
    std::vector<Edge*> sameStateTransitions;

private:
    Ui::tema3finalClass ui;
    Graph graf;
    Node* firstNode = new Node;
    Node* aux;
    Node* draggedNode;
    bool isDragging, isSelected;
    bool isDeterministic;
    bool finishButtonPressed;
};
