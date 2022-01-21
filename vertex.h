#ifndef VERTEX_H
#define VERTEX_H

#include <QObject>
#include <QPoint>
#include <QHash>
#include "edge.h"
class Vertex : public QObject
{
    Q_OBJECT
public:
    explicit Vertex(QPoint pos,QObject *parent = nullptr);
    QPoint pos;
    QHash<Vertex*,Edge*>connEdges;
    int connEdgesNum;
signals:

private:
};

#endif // VERTEX_H
