#include "tema3final.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QApplication>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QLayout>
#include <QInputDialog>
#include <QString>
#include <QMessageBox>

#include <fstream>
#include <iostream>

tema3final::tema3final(QWidget* parent)
    : QMainWindow(parent)
    , isDeterministic(false)
{
    ui.setupUi(this);
    firstNode = new Node();
    aux = new Node();
    //isSelected = false;
    draggedNode = nullptr;
    isDragging = false;
    finishButtonPressed = false;
    connect(ui.checkBox, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(onPushButtonClicked()));
    connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(onPushButton2Clicked()));
    connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(onPushButton3Clicked()));
    ui.lineEdit->setEnabled(false);
}

tema3final::~tema3final()
{}

void tema3final::onCheckBoxStateChanged(int state)
{

    if (state == Qt::Checked)
    {

        qDebug() << "DETERMINISTIC!";
        ui.checkBox->setEnabled(false);
        isDeterministic = true;
    }

}

void tema3final::onPushButtonClicked()
{
    if (finishButtonPressed)
    {
        std::ofstream fout("data.out");

        if (isDeterministic)
        {
            fout << AFD;
        }
        else
        {
            fout << AFN;
        }
    }
    else
        QMessageBox::critical(nullptr, "Error", "The finish button must be pressed before printing the automaton.", QMessageBox::Ok);
}

//verificare daca finishButton e pressed, altfel nu pot utiliza butonu nu checkWord
void tema3final::onPushButton2Clicked()
{
    if (finishButtonPressed)
    {
        QMessageBox messageBox;
        messageBox.setWindowTitle("WORD STATUS");
        messageBox.addButton(QMessageBox::Ok);
        messageBox.setIcon(QMessageBox::Information);

        QString word = ui.lineEdit->text();

        std::string wordFinal = word.toStdString();
        if (isDeterministic)
        {
            if (AFD.CheckWord(wordFinal))
            {
                messageBox.setText("WORD ACCEPTED!");

            }
            else
            {
                messageBox.setText("WORD REJECTED!");
            }
        }
        else
        {
            if (AFN.CheckWord(wordFinal))
            {
                messageBox.setText("WORD ACCEPTED!");
            }
            else
            {
                messageBox.setText("WORD REJECTED!");
            }
        }

        messageBox.exec();
    }
    else
        QMessageBox::critical(nullptr, "Error", "The finish button must be pressed before printing the automaton.", QMessageBox::Ok);
}

void tema3final::onPushButton3Clicked()
{
    if (isDeterministic)
    {
        std::vector<char> finalStates = AFD.getFinalState();
        if (finalStates.size() < 1)
        {
            QMessageBox::critical(nullptr, "Error", "Automaton does not have final states! Add final states before pressing the finish button.", QMessageBox::Ok);
            return;
        }
    }
    else
    {
        std::vector<char> finalStates = AFN.getFinalStates();
        if (finalStates.size() < 1)
        {
            QMessageBox::critical(nullptr, "Error", "Automaton does not have final states! Add final states before pressing the finish button.", QMessageBox::Ok);
            return;
        }
    }
    finishButtonPressed = true;
    ui.lineEdit->setEnabled(true);
}

void tema3final::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton) {
        std::vector<Node*> nodes = graf.getNodes();
        for (Node*& n : nodes) {
            if (fabs(n->getX() - e->pos().x()) < 20 && fabs(n->getY() - e->pos().y()) < 20) {
                draggedNode = n;
                isDragging = true;
                break;
            }
        }
    }
}

void tema3final::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_M)
    {
        if (firstNode)
        {
            char simbol = static_cast<char>('0' + firstNode->getValue());
            if (isDeterministic)
            {
                AFD.deleteSelectedFinalState(simbol);
            }
            else
            {
                AFN.deleteSelectedfinalState(simbol);
            }

            firstNode = aux;
            update();
        }
    }
}



void tema3final::mouseMoveEvent(QMouseEvent* e)
{
    if (isDragging && draggedNode && !finishButtonPressed) {
        QPoint newPos = e->pos();
        draggedNode->setCoordinate(newPos); // modificam coordonatele nodului
        update(); // update visual al grafului
    }
}


void tema3final::mouseReleaseEvent(QMouseEvent* e)
{
    if (!finishButtonPressed)
    {
        int gasit = 0;
        //adaugare noduri
        if (e->button() == Qt::RightButton)
        {
            std::vector<Node*> nodes = graf.getNodes();
            for (Node*& n : nodes)
            {
                if (fabs(n->getX() - e->pos().x()) < 40 && fabs(n->getY() - e->pos().y()) < 40)  //suprapunere noduri
                    return;
            }
            graf.addNode(e->pos());
            nodes = graf.getNodes();
            if (nodes.size() == 1)
            {
                ui.checkBox->setEnabled(false);
                if (isDeterministic)
                {
                    AFD.setQ0(static_cast<char>('0' + nodes[0]->getValue()));
                }
                else
                {
                    AFN.setInitialState(static_cast<char>('0' + nodes[0]->getValue()));
                }
            }
            if (isDeterministic)
            {
                //lucram cu AFD
                nodes = graf.getNodes();
                AFD.addState('0' + nodes[nodes.size() - 1]->getValue());

            }
            else
            {
                nodes = graf.getNodes();
                AFN.addState('0' + nodes[nodes.size() - 1]->getValue());
                //lucram cu AFN
            }
            update();
        }
        else
        {
            //adaugare muchii
            if (e->button() == Qt::LeftButton)
            {
                if (isDragging)
                {
                    isDragging = false;
                    draggedNode = nullptr;
                }
                std::vector<Node*> nodes = graf.getNodes();
                Node* secondNode = nullptr;  // Declare secondNode outside the loop

                for (Node*& n : nodes)
                {
                    if (fabs(n->getX() - e->pos().x()) < 20 && fabs(n->getY() - e->pos().y()) < 20)
                    {
                        gasit = 1;
                        if (firstNode->getValue() == n->getValue())
                        {
                            //add final state
                            if (isDeterministic)
                            {
                                std::vector<char> finalStates = AFD.getFinalState();
                                char stareFinala = '0' + firstNode->getValue();
                                if (std::find(finalStates.begin(), finalStates.end(), stareFinala) == finalStates.end())
                                    AFD.addFinalState(stareFinala);
                                update();
                            }
                            else
                            {
                                std::vector<char> finalStates = AFN.getFinalStates();
                                char stareFinala = '0' + firstNode->getValue();
                                if (std::find(finalStates.begin(), finalStates.end(), stareFinala) == finalStates.end())
                                    AFN.addFinalState(stareFinala);
                                update();
                            }


                        }
                        if (firstNode->getValue() == -1)
                            firstNode = n;
                        else
                        {
                            // Save the second node outside the loop
                            secondNode = n;
                            break;
                        }
                    }
                }
                update();

                // Check if both nodes are valid before showing the cost dialog
                if (firstNode && secondNode/* && firstNode != secondNode*/)
                {
                    // Verify edge conditions
                    std::pair<int, int> edgePair(firstNode->getValue(), secondNode->getValue());
                    std::pair<int, int> edgePair2(secondNode->getValue(), firstNode->getValue());

                    if (firstNode != secondNode)
                    {
                        if (addedEdges.find(edgePair) == addedEdges.end())
                        {
                            bool ok;
                            QString costStr = QInputDialog::getText(this, tr("Enter Transition Character"), tr("Character:"), QLineEdit::Normal, "", &ok);

                            // Check if user pressed OK and the input is not empty
                            if (ok && !costStr.isEmpty()) {
                                // Convert the QString to char
                                char cost = costStr.at(0).toLatin1(); // or use costStr[0].toLatin1()
                                int gasit2 = 0;
                                //verific daca este determinist, si daca e determinist nu permit sa se adauge o alta muchie cu un simbol deja folosit in acea stare
                                if (isDeterministic)
                                {
                                    std::vector<Edge*> edges = graf.getEdges();
                                    for (int i = 0; i < edges.size() && gasit2 == 0; i++)
                                        if (edges[i]->getFirstNode() == firstNode && edges[i]->getCost() == cost)
                                        {
                                            QMessageBox::critical(nullptr, "Error", "Automaton is deterministic!", QMessageBox::Ok);
                                            gasit2 = 1;
                                            return;
                                        }
                                }
                                // Add the edge with cost
                                if (gasit2 == 0)
                                    graf.addEdge(firstNode, secondNode, cost);
                                if (isDeterministic)
                                {
                                    std::vector<char> alphabet = AFD.getSigma();
                                    if (std::find(alphabet.begin(), alphabet.end(), cost) == alphabet.end())
                                        AFD.addSigma(cost);

                                    char q, a, p;
                                    q = static_cast<char>('0' + firstNode->getValue());
                                    a = cost;
                                    p = static_cast<char>('0' + secondNode->getValue());
                                    AFD.addToDelta(q, a, p);
                                }
                                else
                                {
                                    std::vector<char> alphabet = AFN.getAlphabet();
                                    if (std::find(alphabet.begin(), alphabet.end(), cost) == alphabet.end())
                                        AFN.addSigma(cost);

                                    char q, a, p;
                                    q = static_cast<char>('0' + firstNode->getValue());
                                    a = cost;
                                    p = static_cast<char>('0' + secondNode->getValue());
                                    AFN.addToDelta(q, a, p);
                                }
                                addedEdges.insert(edgePair);
                                update();
                                firstNode = aux;
                            }
                            // Handle the case where ok is false or costStr is empty

                        }
                    }
                    else
                    {
                        if (1)
                        {
                            bool ok;
                            QString costStr = QInputDialog::getText(this, tr("Enter Transition Character"), tr("Character:"), QLineEdit::Normal, "", &ok);

                            // Check if user pressed OK and the input is not empty
                            if (ok && !costStr.isEmpty()) {
                                // Convert the QString to char
                                char cost = costStr.at(0).toLatin1(); // or use costStr[0].toLatin1()
                                int gasit2 = 0;
                                //verific daca este determinist, si daca e determinist nu permit sa se adauge o alta muchie cu un simbol deja folosit in acea stare
                                if (isDeterministic)
                                {
                                    std::vector<Edge*> edges = graf.getEdges();
                                    for (int i = 0; i < edges.size() && gasit2 == 0; i++)
                                        if (edges[i]->getFirstNode() == firstNode && edges[i]->getCost() == cost)
                                        {
                                            QMessageBox::critical(nullptr, "Error", "Automaton is deterministic!", QMessageBox::Ok);
                                            gasit2 = 1;
                                            return;
                                        }
                                }
                                // Add the edge with cost
                                if (gasit2 == 0)
                                {
                                    graf.addEdge(firstNode, secondNode, cost);
                                    Edge* ed = new Edge(firstNode, secondNode, cost);
                                    sameStateTransitions.push_back(ed);
                                }
                                if (isDeterministic)
                                {
                                    std::vector<char> alphabet = AFD.getSigma();
                                    if (std::find(alphabet.begin(), alphabet.end(), cost) == alphabet.end())
                                        AFD.addSigma(cost);

                                    char q, a, p;
                                    q = static_cast<char>('0' + firstNode->getValue());
                                    a = cost;
                                    p = static_cast<char>('0' + secondNode->getValue());
                                    AFD.addToDelta(q, a, p);
                                }
                                else
                                {
                                    std::vector<char> alphabet = AFN.getAlphabet();
                                    if (std::find(alphabet.begin(), alphabet.end(), cost) == alphabet.end())
                                        AFN.addSigma(cost);

                                    char q, a, p;
                                    q = static_cast<char>('0' + firstNode->getValue());
                                    a = cost;
                                    p = static_cast<char>('0' + secondNode->getValue());
                                    AFN.addToDelta(q, a, p);
                                }
                                addedEdges.insert(edgePair);
                                update();
                                firstNode = aux;
                            }
                            // Handle the case where ok is false or costStr is empty

                        }
                    }
                }
                if (gasit == 0)
                {
                    firstNode = aux;
                    update();
                }
            }
        }
    }
    else
        QMessageBox::critical(nullptr, "Error", "The automaton can no longer be modified.", QMessageBox::Ok);
}

void tema3final::paintEvent(QPaintEvent* e)
{
    std::vector<Edge*> drawnEdges;
    QPainter p(this);
    std::vector<Node*> nodes = graf.getNodes();
    std::vector<char> finalStates;
    if (isDeterministic)
    {
        finalStates = AFD.getFinalState();
    }
    else
    {
        finalStates = AFN.getFinalStates();
    }

    // Draw nodes
    for (Node*& n : nodes)
    {
        int gasit = 0;
        if (n->getValue() == 0)
        {
            p.setBrush(Qt::red);
        }
        for (int j = 0; j < finalStates.size() && gasit == 0; j++)
        {
            if (finalStates[j] == ('0' + n->getValue()))gasit = 1;
        }

        if (gasit)
        {
            QRect r2(n->getX() - 30, n->getY() - 30, 60, 60);
            p.drawEllipse(r2);
        }
        QRect r(n->getX() - 20, n->getY() - 20, 40, 40);
        p.drawEllipse(r);
        QString s;
        s.setNum(n->getValue());
        p.drawText(r, Qt::AlignCenter, s);
        p.setBrush(Qt::white);
    }

    std::vector<Edge*> edges = graf.getEdges();

    // Draw edges
    for (Edge*& ed : edges)
    {
        double x1 = ed->getFirstNode()->getX();
        double y1 = ed->getFirstNode()->getY();
        double x2 = ed->getSecondNode()->getX();
        double y2 = ed->getSecondNode()->getY();
        double halfWidth1 = 20; // Half of the first node's width
        double halfWidth2 = 20; // Half of the second node's width

        // Calculate the direction vector from the first node to the second node.
        double dx = x2 - x1;
        double dy = y2 - y1;

        // Calculate the distance between the centers of the two nodes.
        double distance = std::sqrt(dx * dx + dy * dy);

        // Calculate the unit vector in the direction from the first node to the second node.
        double unitX = dx / distance;
        double unitY = dy / distance;

        // Calculate the intersection points with the boundaries of the nodes.
        double x1_edge = x1 + halfWidth1 * unitX;
        double y1_edge = y1 + halfWidth1 * unitY;
        double x2_edge = x2 - halfWidth2 * unitX;
        double y2_edge = y2 - halfWidth2 * unitY;

        // Draw the line from the calculated points.
        p.drawLine(x1_edge, y1_edge, x2_edge, y2_edge);

        int gasit = 0;
        for (int i = 0; i < drawnEdges.size() && gasit == 0; i++)
        {
            if (drawnEdges[i]->getFirstNode() == ed->getSecondNode()
                && drawnEdges[i]->getSecondNode() == ed->getFirstNode())gasit = 1;
        }

        // Draw the cost near the middle point of the edge
        double midX = (x1_edge + x2_edge) / 2;
        double midY = (y1_edge + y2_edge) / 2;

        QFont originalFont = p.font();

        char cost = ed->getCost(); // Assuming getCost() returns a char
        QString costStr = QString(cost); // Convert char to QString directly



        if (gasit == 1)
        {
            QRect costRect(midX, midY, 20, 20); // Adjusted for centering
            QFont font = p.font();
            font.setWeight(QFont::Bold);
            font.setPixelSize(16);
            p.setFont(font);

            p.drawText(costRect, Qt::AlignCenter, costStr);
        }
        else
        {
            QRect costRect(midX - 20, midY - 20, 20, 20); // Adjusted for centering
            QFont font = p.font();
            font.setWeight(QFont::Bold);
            font.setPixelSize(16);
            p.setFont(font);

            p.drawText(costRect, Qt::AlignCenter, costStr);
        }

        // Set the font weight to make the text thicker

        p.setFont(originalFont);

        double arrowSize = 15.0;  // Adjust the size of the arrowhead as needed
        double angle = atan2(y2_edge - y1_edge, x2_edge - x1_edge);
        double arrowX1 = x2_edge - arrowSize * cos(angle - M_PI / 6);
        double arrowY1 = y2_edge - arrowSize * sin(angle - M_PI / 6);
        double arrowX2 = x2_edge - arrowSize * cos(angle + M_PI / 6);
        double arrowY2 = y2_edge - arrowSize * sin(angle + M_PI / 6);

        // Draw the arrowhead (a triangle) at the endpoint of the line.
        p.drawLine(x2_edge, y2_edge, arrowX1, arrowY1);
        p.drawLine(x2_edge, y2_edge, arrowX2, arrowY2);


        drawnEdges.push_back(ed);
    }

    for (const auto ed : sameStateTransitions)
    {
        double x = ed->getFirstNode()->getX();
        double y = ed->getFirstNode()->getY();


        QFont originalFont = p.font();

        char cost = ed->getCost(); // Assuming getCost() returns a char
        QString costStr = QString(cost); // Convert char to QString directly

        QRect costRect(x + 20, y, 20, 20); // Adjusted for centering
        QFont font = p.font();
        font.setWeight(QFont::Bold);
        font.setPixelSize(16);
        p.setFont(font);

        p.drawText(costRect, Qt::AlignCenter, costStr);
    }

    if (firstNode->getValue() != -1)
    {
        QPen pen;
        pen.setColor(Qt::green);
        pen.setWidth(2);
        p.setPen(pen);
        QRect r(firstNode->getX() - 20, firstNode->getY() - 20, 40, 40);
        p.drawEllipse(r);
        QString s;
        s.setNum(firstNode->getValue());
        p.drawText(r, Qt::AlignCenter, s);
    }
}
