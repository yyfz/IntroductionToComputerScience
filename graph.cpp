#include "graph.h"
#include <QtDebug>
#include <QPixmap>
#include <QBitmap>
#include <QMouseEvent>
#include <QPainter>
#include <QtMath>
#include <algorithm>

Vertex::Vertex(QPoint pos,QWidget *parent,VertexInfo *info)
{
    this->setFrameShape(Shape::NoFrame);
    showSize = 20*parent->width()/1000;
    setStyleSheet("min-width: "+QString::number(showSize)+"; min-height: "+QString::number(showSize)+";max-width:"+QString::number(showSize)+"; max-height:"+QString::number(showSize)+"px;border-radius:"+QString::number(showSize/2)+"; background:skyblue");
    this->info = info;
    this->pos = pos;
    nameLabel = new QLabel(parent);
    if(info) {
        nameLabel->setText(info->name);
    }else nameLabel->setText("");
    installEventFilter(this);
    setAttribute(Qt::WA_Hover,true);
    isDrawing = false;
    canExtendEdge = true;
    isTarget = false;
    isDragging = false;
    showDetailTimer = 0;
    setParent(parent);
    this->parent = parent;
    resize(20*parent->width()/1000,20*parent->height()/1000);
    relativePos.setX((double)pos.x()/parent->size().width());
    relativePos.setY((double)pos.y()/parent->size().height());
    move(relativePos.x()*parent->size().width()-size().width()/2,relativePos.y()*parent->size().height()-size().height()/2);
    enterAnimation = new QPropertyAnimation(this,"");
    enterAnimation->setStartValue(0);
    enterAnimation->setEndValue(30);
    enterAnimation->setDuration(200);
    connect(enterAnimation,&QPropertyAnimation::valueChanged,this,&Vertex::enterImageChanged);
    leaveAnimation = new QPropertyAnimation(this,"");
    leaveAnimation->setStartValue(0);
    leaveAnimation->setEndValue(30);
    leaveAnimation->setDuration(200);
    connect(leaveAnimation,&QPropertyAnimation::valueChanged,this,&Vertex::leaveImageChanged);
    blinkAnimation = new QPropertyAnimation(this,"");
    blinkAnimation->setDuration(800);
    blinkAnimation->setStartValue(0);
    blinkAnimation->setEndValue(90);
    connect(blinkAnimation,&QPropertyAnimation::valueChanged,[this](QVariant value){
        if(value.toInt()<=30){
            showSize = 20*this->parent->width()*(1.0+value.toFloat()/100)/1000;
            updateSize();
            move(relativePos.x()*this->parent->width()-showSize/2,relativePos.y()*this->parent->height()-showSize/2);
        }
        else if(value.toInt()>=60){
            showSize = 20*this->parent->width()*(1.3-(value.toFloat()-70)/100)/1000;
            updateSize();
            move(relativePos.x()*this->parent->width()-showSize/2,relativePos.y()*this->parent->height()-showSize/2);
        }
    });
}

void Vertex::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && !isDrawing && !canExtendEdge){
        if(!isTarget){
            emit lockTargetVertex(this);
            isTarget = true;
            setStyleSheet("min-width: "+QString::number(showSize)+"; min-height: "+QString::number(showSize)+";max-width:"+QString::number(showSize)+"; max-height:"+QString::number(showSize)+"px;border-radius:"+QString::number(showSize/2)+"; background:green");
        }
        offset = event->globalPos()-QPoint(relativePos.x()*parent->width()-width()/2,relativePos.y()*parent->height()-height()/2);
        emit lockEndVertex(this);
        return;
    }
    if(event->button() == Qt::LeftButton){
        if(!isTarget){
            isTarget = true;
            setStyleSheet("min-width: "+QString::number(showSize)+"; min-height: "+QString::number(showSize)+";max-width:"+QString::number(showSize)+"; max-height:"+QString::number(showSize)+"px;border-radius:"+QString::number(showSize/2)+"; background:green");
            emit lockTargetVertex(this);
        }
        offset = event->globalPos()-QPoint(relativePos.x()*parent->width()-width()/2,relativePos.y()*parent->height()-height()/2);
        isDragging = true;
    }else if(event->button() == Qt::RightButton){
        if(!isDrawing && !isPrinted)
            emit deleteSignal(this);
    }
}

void Vertex::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        if(isMoving){
            isDragging = false;
            isMoving = false;
            return;
        }
        if(isDoubleClicking != 1){
            isDoubleClicking = startTimer(60);
        }
        else isDoubleClicking = 0;
        if(isDragging) {
            isDragging = false;
            return;
        }
    }
}

void Vertex::mouseMoveEvent(QMouseEvent *event)
{
    if(isDragging){
        isMoving = true;
        if(!info->isHidden())
            info->hide();
        if(event->buttons() & Qt::LeftButton){
            move(event->globalPos() - offset);
            this->pos = event->globalPos() - offset + QPoint(width()/2,height()/2);
            this->relativePos.setX(((double)this->pos.x()/parent->size().width()));
            this->relativePos.setY(((double)this->pos.y()/parent->size().height()));
            this->nameLabel->move(relativePos.x()*parent->width()-nameLabel->width()/2,relativePos.y()*parent->height()+height());
            update();
            emit draggingSignal();
        }
    }
}

bool Vertex::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == this){
        if (event->type() == QEvent::HoverEnter){
            if(showDetailTimer == 0)
                showDetailTimer = startTimer(500);
            enterAnimation->start();
            emit mouseEnterHover(this);
            return true;
        }else if (event->type() == QEvent::HoverLeave){
            leaveAnimation->start();
            if(!info->isHidden())
                info->hide();
            showDetailTimer = 0;
            emit mouseLeaveHover();
            return true;
        }else return false;
    }
    return eventFilter(obj,event);
}

void Vertex::mouseDoubleClickEvent(QMouseEvent *event)
{
    isDoubleClicking = 1;
    if(!isDrawing && canExtendEdge){
        isDrawing = true;
        emit mouseDoubleClick(this);
    }else if(!isDrawing && !canExtendEdge){
        emit lockEndVertex(this);
    }
}

void Vertex::clear()
{
    VertexInfo *p = info;
    delete p;
    info = nullptr;
    nameLabel->hide();
    QLabel *p1 = nameLabel;
    delete p1;
    nameLabel = nullptr;
    QPropertyAnimation *p2 = enterAnimation;
    delete p2;
    enterAnimation = nullptr;
    p2 = leaveAnimation;
    delete p2;
    leaveAnimation = nullptr;
    if(content){
        Content *p = content;
        delete p;
        content = nullptr;
    }
}

void Vertex::updateSize()
{
    if(isTarget)
        setStyleSheet("min-width: "+QString::number(showSize)+"; min-height: "+QString::number(showSize)+";max-width:"+QString::number(showSize)+"; max-height:"+QString::number(showSize)+"px;border-radius:"+QString::number(showSize/2)+"; background:green");
    else
        setStyleSheet("min-width: "+QString::number(showSize)+"px; min-height: "+QString::number(showSize)+"px;max-width:"+QString::number(showSize)+"px; max-height:"+QString::number(showSize)+"px;border-radius:"+QString::number(showSize/2)+"; background:skyblue");
}

void Vertex::enterImageChanged(QVariant value)
{
    showSize = 20*(1+value.toFloat()/100)*parent->width()/1000;
    updateSize();
    move(relativePos.x()*parent->width()-showSize/2,relativePos.y()*parent->height()-showSize/2);
}

void Vertex::leaveImageChanged(QVariant value)
{
    showSize = 20*parent->width()*(1.3-value.toFloat()/100)/1000;
    updateSize();
    move(relativePos.x()*parent->width()-showSize/2,relativePos.y()*parent->height()-showSize/2);
}

void Vertex::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == showDetailTimer){
        info->resize(300*parent->width()/1000,300*parent->height()/1000);
        if(cursor().pos().x()-info->width()/2>0 && cursor().pos().y()-height()-info->height()>0)
            info->move(cursor().pos().x()-info->width()/2,cursor().pos().y()-height()-info->height());
        else
            info->move(cursor().pos().x()-info->width()/2,cursor().pos().y()+height());
        info->updateInfo();
        info->show();
        showDetailTimer = 0;
    }else if (event->timerId() == isDoubleClicking){
        isDoubleClicking = 0;
        if(num == -1)
            content->show();
    }

}


Edge::Edge(QObject *parent,Vertex *vl,Vertex *vr,double weight,int edgeType)
{
    this->vl = vl;
    this->vr = vr;
    this->weight = weight;
    this->edgeType = edgeType;
}

Graph::Graph(QObject *parent,int graphType) : QObject(parent)
{
    this->graphType = graphType;
}


void Graph::CreateVertex(QPoint pos,QWidget *parent,VertexInfo *info)
{
    Vertex *v = new Vertex(pos,parent,info);
    AllVertex.push_back(v);
    qDebug() << "成功创建点！  坐标" << pos.x() << "," << pos.y();
}

bool Graph::CreateEdge(Vertex *v1, Vertex *v2,double weight,QWidget *parent)
{
    for(QLinkedList<Edge*>::const_iterator it = AllEdge.begin();it != AllEdge.end();it++){
        if(graphType == UNDIRECTED){
            if(((*it)->vl == v1 && (*it)->vr == v2) || ((*it)->vl == v2 && (*it)->vr == v1)){
                qDebug() << "该边已存在！";
                return false;
            }
        }else{
            if(((*it)->vl == v1 && (*it)->vr == v2)){
                qDebug() << "该边已存在！ DIRECTED";
                return false;
            }
        }
    }
    Edge *e = new Edge(this,v1,v2,weight);
    v1->connEdges.insert(v2,e);
    if(graphType == UNDIRECTED)
        v2->connEdges.insert(v1,e);
    AllEdge.push_back(e);
    qDebug() << "成功创建边！";
    return true;
}

void Graph::deleteVertex(Vertex *v)
{
    AllVertex.erase(qFind(AllVertex.begin(),AllVertex.end(),v));
    v->clear();
    if(graphType == UNDIRECTED){
        for(QHash<Vertex*,Edge*>::iterator it = v->connEdges.begin();it != v->connEdges.end();it++){
            AllEdge.erase(qFind(AllEdge.begin(),AllEdge.end(),(*it)));
            Vertex *tmp = (*it)->vl == v ? (*it)->vr:(*it)->vl;
            tmp->connEdges.erase(qFind(tmp->connEdges.begin(),tmp->connEdges.end(),*it));
            delete *it;
        }
        delete v;
    }else{
        for(QLinkedList<Edge*>::const_iterator it = AllEdge.begin();it != AllEdge.end();it++){
            if((*it)->vl == v || (*it)->vr == v){
                AllEdge.erase(qFind(AllEdge.begin(),AllEdge.end(),(*it)));
                if((*it)->vr == v)
                    (*it)->vl->connEdges.erase(qFind((*it)->vl->connEdges.begin(),(*it)->vl->connEdges.end(),*it));
                delete *it;
            }
        }
        delete v;
    }

}

void Graph::clear()
{
    for(QLinkedList<Vertex*>::iterator it = AllVertex.begin();it != AllVertex.end();it++){
        (*it)->clear();
        delete (*it);
    }
    AllVertex.clear();
    for(QLinkedList<Edge*>::iterator it = AllEdge.begin();it != AllEdge.end();it++){
        delete (*it);
    }
    AllEdge.clear();
}
