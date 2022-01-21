#ifndef MYCANVAS_H
#define MYCANVAS_H

#include <QWidget>
#include <QLinkedList>
#include <math.h>
#include <QPen>
#include "graph.h"
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>

class MyCanvas : public QFrame
{
    Q_OBJECT

public:
    explicit MyCanvas(QWidget *parent = nullptr);
    ~MyCanvas();
    bool eventFilter(QObject *obj,QEvent *event);
    void paintEvent(QPaintEvent *);
    Vertex *targetVertex;
    Graph *g;
    void clear();
    void ReadFile(QString filename);
    void ReadVertexInfo(QString filename);
    void createVertexOnCanvas(QPoint pos);
    QPropertyAnimation *printHelloWorldAnimation;
    QPropertyAnimation *printCoursesDiagramAnimation;
    QPropertyAnimation *printContentAnimation;
    void showHelloWorld(QVariant value);
    void showCoursesDiagram(QVariant value);
    void showContent(QVariant value);
    bool isPrinting = false;
    int countVertex = 0;
    int countEdge = 0;
    bool edgeFlag = true;
    QPointF startPrintingPoint;
    QPointF endPrintingPoint;
protected:
protected slots:
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
signals:
    void dragging();
    void getTargetVertex(Vertex*);
    void loseTargetVertex();
private:
    Vertex* startVertex;
    Vertex* endVertex;
    QPointF endPoint;
    bool startDraw;
    bool isFocused;
    bool isDragging;
    QPoint offset;
    QPen pen1;
    QPen pen2;
    QPen pen3;
    float l = 20.0;
    float a =0.3;
};

#endif // MYCANVAS_H
