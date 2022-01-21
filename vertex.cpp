#include "vertex.h"

Vertex::Vertex(QPoint pos,QObject *parent) : QObject(parent)
{
    this->pos = pos;
    connEdgesNum = 0;
}
