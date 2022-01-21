#ifndef GRAPH_H
#define GRAPH_H

#include <QObject>
#include <QPoint>
#include <QHash>
#include <QLabel>
#include <QLinkedList>
#include "vertexinfo.h"
#include <QPropertyAnimation>
#include <QSignalTransition>
#include <QState>
#include "content.h"

class Edge;

class Vertex : public QLabel
{
    Q_OBJECT

public:
    explicit Vertex(QPoint pos,QWidget *parent = nullptr,VertexInfo *info = nullptr);
    QPoint pos;
    QHash<Vertex*,Edge*>connEdges;
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    bool eventFilter(QObject *obj,QEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    bool isDrawing;
    bool canExtendEdge;
    QPointF relativePos;
    VertexInfo *info;
    QLabel *nameLabel;
    bool isTarget;
    void clear();
    QState *vertexHoverEnter = nullptr;
    QState *vertexHoverLeave = nullptr;
    int showSize;
    void updateSize();
    void enterImageChanged(QVariant value);
    void leaveImageChanged(QVariant value);
    QPropertyAnimation *enterAnimation;
    QPropertyAnimation *leaveAnimation;
    QPropertyAnimation *blinkAnimation;
    bool isPrinted = false;
    int num = 0;
    Content* content = nullptr;
    QLabel *clickImg = nullptr;
protected:
    void timerEvent(QTimerEvent *event);
signals:
    void mouseDoubleClick(Vertex*);
    void mouseMove(QPoint pos);
    void mouseEnterHover(Vertex*);
    void mouseLeaveHover();
    void lockEndVertex(Vertex*);
    void draggingSignal();
    void deleteSignal(Vertex*);
    void lockTargetVertex(Vertex*);
private:
    QPoint offset;
    QWidget* parent;
    bool isDragging;
    int showDetailTimer;
    int isDoubleClicking = 0;
    bool isMoving = false;
};

class Edge : public QLabel
{
    Q_OBJECT
private:
    enum{
       DIRECTED,
       UNDIRECTED
    };
public:
    explicit Edge(QObject *parent,Vertex *vl,Vertex *vr,double weight,int edgeType = UNDIRECTED);
    Vertex *vl;
    Vertex *vr;
    double weight;
    int edgeType;
    bool afterPrint = true;

signals:
private:

};


class Graph : public QObject
{
    Q_OBJECT
public:
    enum{
        DIRECTED,
        UNDIRECTED
    };
    explicit Graph(QObject *parent = nullptr,int graphType = UNDIRECTED);
    QLinkedList<Vertex*>AllVertex;
    QLinkedList<Edge*>AllEdge;
    void CreateVertex(QPoint pos,QWidget *parent,VertexInfo *info = nullptr);
    bool CreateEdge(Vertex *v1,Vertex *v2,double weight = 1,QWidget *parent = nullptr);
    void deleteVertex(Vertex* v);
    int graphType;
    void clear();

signals:


};

#endif // GRAPH_H
