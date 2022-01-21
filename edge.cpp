#include "edge.h"

Edge::Edge(QObject *parent,Vertex *vl,Vertex *vr,double weight) : QObject(parent)
{
    this->vl = vl;
    this->vr = vr;
    this->weight = weight;
}
