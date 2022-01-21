#ifndef EDGE_H
#define EDGE_H

#include <QObject>
#include "vertex.h"

class Edge : public QObject
{
    Q_OBJECT
public:
    explicit Edge(QObject *parent,Vertex *vl,Vertex *vr,double weight);
    Vertex *vl;
    Vertex *vr;
    double weight;

signals:

private:
};

#endif // EDGE_H
