#include "mycanvas.h"
#include "ui_mycanvas.h"
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QLinkedList>
#include <QBitmap>
#include "widget.h"
#include <QPalette>

MyCanvas::MyCanvas(QWidget *parent) :
    QFrame(parent)
{
    setStyleSheet("background-color: rgb(255,255,255);");
    resize(1000,1000);
    move(-150,-150);
    g=new Graph(this,Graph::UNDIRECTED);
    installEventFilter(this);
    startDraw = false;
    setMouseTracking(true);
    setCursor(QCursor(Qt::CursorShape::CrossCursor));
    isFocused = false;
    startVertex = nullptr;
    endVertex = nullptr;
    isDragging = false;
    targetVertex = nullptr;
    pen1.setBrush(QBrush(QColor(21,198,160)));
    pen2.setBrush(QBrush(QColor(60,183,208)));
    pen3.setBrush(QBrush(QColor(111,111,111)));
    pen1.setStyle(Qt::PenStyle::DashLine);
    pen2.setStyle(Qt::PenStyle::DotLine);
    pen3.setStyle(Qt::PenStyle::DotLine);
    pen1.setCapStyle(Qt::PenCapStyle::RoundCap);
    pen2.setCapStyle(Qt::PenCapStyle::RoundCap);
    pen3.setCapStyle(Qt::PenCapStyle::RoundCap);
    pen1.setJoinStyle(Qt::PenJoinStyle::RoundJoin);
    pen2.setJoinStyle(Qt::PenJoinStyle::RoundJoin);
    pen3.setJoinStyle(Qt::PenJoinStyle::RoundJoin);
    pen1.setWidth(3);
    pen2.setWidth(3);
    pen3.setWidth(5);
    printHelloWorldAnimation = new QPropertyAnimation(this,"");
    printHelloWorldAnimation->setStartValue(0);
    printHelloWorldAnimation->setEndValue(11);
    printHelloWorldAnimation->setDuration(200);
    connect(printHelloWorldAnimation,&QPropertyAnimation::valueChanged,this,&MyCanvas::showHelloWorld);
    printCoursesDiagramAnimation = new QPropertyAnimation(this,"");
    printCoursesDiagramAnimation->setStartValue(0);
    printCoursesDiagramAnimation->setEndValue(11);
    printCoursesDiagramAnimation->setDuration(200);
    connect(printCoursesDiagramAnimation,&QPropertyAnimation::valueChanged,this,&MyCanvas::showCoursesDiagram);
    printContentAnimation = new QPropertyAnimation(this,"");
    printContentAnimation->setStartValue(0);
    printContentAnimation->setEndValue(11);
    printContentAnimation->setDuration(200);
    connect(printContentAnimation,&QPropertyAnimation::valueChanged,this,&MyCanvas::showContent);
}

MyCanvas::~MyCanvas()
{
}

bool MyCanvas::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == this){
        if(event->type() == QEvent::MouseButtonRelease){
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() == Qt::LeftButton){
                if(isDragging){
                    isDragging = false;
                    if(cursor().shape()!=Qt::CrossCursor)
                        setCursor(QCursor(Qt::CrossCursor));
                    return true;
                }else return false;
            }else if(mouseEvent->button() == Qt::RightButton){
                if(startDraw){
                    startVertex->isDrawing = false;
                    startVertex = nullptr;
                    endVertex = nullptr;
                    startDraw = false;
                    qDebug() << "Blank is pressed!";
                    qDebug() << "End drawing!";
                    update();
                    return true;
                }else return false;
            }else return false;
        }else if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() == Qt::LeftButton){
                offset = mouseEvent->globalPos()-pos();
                isDragging = true;
                if(cursor().shape()!=Qt::ClosedHandCursor)
                    setCursor(QCursor(Qt::ClosedHandCursor));
                if(targetVertex){
                    targetVertex = nullptr;
                    emit loseTargetVertex();
                }
                return true;
            }else return false;
        }else if(event->type() == QEvent::MouseButtonDblClick){
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() == Qt::LeftButton){
                if(!startDraw){
                    createVertexOnCanvas(mouseEvent->pos());
                    return true;
                }else{
                    createVertexOnCanvas(mouseEvent->pos());
                    isFocused = true;
                    endVertex = g->AllVertex.back();
                    update();
                    return true;
                }
            }else return false;
        }else return false;
    }else return false;
    return eventFilter(obj,event);
}

void MyCanvas::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    for(QLinkedList<Vertex*>::iterator it = g->AllVertex.begin();it != g->AllVertex.end();it++){
        if((*it)->num == -1){
             QFont font("华光行书_CNKI",9*width()/1000,-1,true);
             QPen pen4 = pen3;
             painter.setFont(font);
             pen4.setWidth(3);
             pen4.setStyle(Qt::PenStyle::SolidLine);
             painter.setPen(pen4);
             painter.drawText((*it)->relativePos*width()+QPoint(20,-20)*width()/1000,"点击！");
             float x1 = ((*it)->relativePos*width()+QPoint(20,-20)*width()/1000).x();
             float y1 = ((*it)->relativePos*width()+QPoint(20,-20)*width()/1000).y();
             float x2 = ((*it)->relativePos*width()+QPoint(10,-10)*width()/1000).x();
             float y2 = ((*it)->relativePos*width()+QPoint(10,-10)*width()/1000).y();
             float x3 = x2 - l/2 * cos(atan2((y2 - y1) , (x2 - x1)) - a);
             float y3 = y2 - l/2 * sin(atan2((y2 - y1) , (x2 - x1)) - a);
             float x4 = x2 - l/2 * sin(atan2((x2 - x1) , (y2 - y1)) - a);
             float y4 = y2 - l/2 * cos(atan2((x2 - x1) , (y2 - y1)) - a);
             painter.drawLine(x2,y2,x3,y3);
             painter.drawLine(x2,y2,x4,y4);
             painter.drawLine(x1,y1,x2,y2);
        }
    }
    pen3.setWidth(3);
    painter.setPen(pen3);
    if(isPrinting){
        if(g->graphType == Graph::UNDIRECTED)
            painter.drawLine(startPrintingPoint,endPrintingPoint);
        else{
            float x1 = startPrintingPoint.x();
            float y1 = startPrintingPoint.y();
            float x2 = endPrintingPoint.x();
            float y2 = endPrintingPoint.y();
            float x3 = x2 - l * cos(atan2((y2 - y1) , (x2 - x1)) - a);
            float y3 = y2 - l * sin(atan2((y2 - y1) , (x2 - x1)) - a);
            float x4 = x2 - l * sin(atan2((x2 - x1) , (y2 - y1)) - a);
            float y4 = y2 - l * cos(atan2((x2 - x1) , (y2 - y1)) - a);
            painter.drawLine(x2,y2,x3,y3);
            painter.drawLine(x2,y2,x4,y4);
            painter.drawLine(x1,y1,x2,y2);
        }
    }
    painter.setPen(pen1);
    for(QLinkedList<Edge*>::const_iterator it = g->AllEdge.begin();it != g->AllEdge.end();it++){
        if(!isPrinting || (isPrinting && (*it)->afterPrint)){
            if(g->graphType == Graph::UNDIRECTED)
                painter.drawLine((*it)->vl->relativePos*this->width(),(*it)->vr->relativePos*this->height());
            else{
                float x1 = (*it)->vl->relativePos.x()*this->width();
                float y1 = (*it)->vl->relativePos.y()*this->width();
                float x2 = (*it)->vr->relativePos.x()*this->width()
                        -(*it)->vr->width()/2*cos(atan2(((*it)->vr->relativePos.y()*this->width() - y1)
                                                      , ((*it)->vr->relativePos.x()*this->width() - x1)));
                float y2 = (*it)->vr->relativePos.y()*this->width()
                        -(*it)->vr->height()/2*sin(atan2(((*it)->vr->relativePos.y()*this->width() - y1)
                                                       , ((*it)->vr->relativePos.x()*this->width() - x1)));
                float x3 = x2 - l * cos(atan2((y2 - y1) , (x2 - x1)) - a);
                float y3 = y2 - l * sin(atan2((y2 - y1) , (x2 - x1)) - a);
                float x4 = x2 - l * sin(atan2((x2 - x1) , (y2 - y1)) - a);
                float y4 = y2 - l * cos(atan2((x2 - x1) , (y2 - y1)) - a);
                painter.drawLine(x2,y2,x3,y3);
                painter.drawLine(x2,y2,x4,y4);
                painter.drawLine(x1,y1,x2,y2);
            }
        }
    }
    painter.setPen(pen2);
    if(startDraw && !isFocused){
        if(g->graphType == Graph::UNDIRECTED)
            painter.drawLine(startVertex->relativePos*this->size().width(),endPoint);
        else{
            float x1 = startVertex->relativePos.x()*this->width();
            float y1 = startVertex->relativePos.y()*this->width();
            float x2 = endPoint.x();
            float y2 = endPoint.y();
            float x3 = x2 - l * cos(atan2((y2 - y1) , (x2 - x1)) - a);
            float y3 = y2 - l * sin(atan2((y2 - y1) , (x2 - x1)) - a);
            float x4 = x2 - l * sin(atan2((x2 - x1) , (y2 - y1)) - a);
            float y4 = y2 - l * cos(atan2((x2 - x1) , (y2 - y1)) - a);
            painter.drawLine(x2,y2,x3,y3);
            painter.drawLine(x2,y2,x4,y4);
            painter.drawLine(x1,y1,x2,y2);
        }
    }
    if(isFocused && startDraw){
        if(g->CreateEdge(startVertex,endVertex,1,this)) {
            painter.setPen(pen1);
            if(g->graphType == Graph::UNDIRECTED){
                painter.drawLine(startVertex->relativePos*this->size().width(),endVertex->relativePos*this->size().width());
            }else{
                float x1 = startVertex->relativePos.x()*this->width();
                float y1 = startVertex->relativePos.y()*this->width();
                float x2 = endVertex->relativePos.x()*this->width()
                        -endVertex->width()/2*cos(atan2((endVertex->relativePos.y()*this->width() - y1)
                                                      , (endVertex->relativePos.x()*this->width() - x1)));
                float y2 = endVertex->relativePos.y()*this->width()
                        -endVertex->height()/2*sin(atan2((endVertex->relativePos.y()*this->width() - y1)
                                                       , (endVertex->relativePos.x()*this->width() - x1)));
                float x3 = x2 - l * cos(atan2((y2 - y1) , (x2 - x1)) - a);
                float y3 = y2 - l * sin(atan2((y2 - y1) , (x2 - x1)) - a);
                float x4 = x2 - l * sin(atan2((x2 - x1) , (y2 - y1)) - a);
                float y4 = y2 - l * cos(atan2((x2 - x1) , (y2 - y1)) - a);
                painter.drawLine(x2,y2,x3,y3);
                painter.drawLine(x2,y2,x4,y4);
                painter.drawLine(x1,y1,x2,y2);
            }
            g->AllEdge.back()->setParent(this);
        }
        startVertex->isDrawing = false;
        startVertex = nullptr;
        endVertex = nullptr;
        startDraw = false;
        isFocused = false;
        qDebug() << "End drawing!";
    }
    painter.setPen(pen3);
    pen3.setWidth(5);
    painter.drawRect(0,0,size().width(),size().height());
}

void MyCanvas::clear()
{
    startDraw = false;
    isFocused = false;
    startVertex = nullptr;
    endVertex = nullptr;
    isDragging = false;
    targetVertex = nullptr;
}

void MyCanvas::ReadFile(QString filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << file.errorString();
        return;
    }
    bool printV = false, printE = false;
    while(!file.atEnd()){
        QByteArray line = file.readLine();
        if(line == "vertex:\n" || line == "\xEF\xBB\xBFvertex:\n") {
            printV = true;
            continue;
        }
        if(line == "edge:\n"){
            printV = false;
            printE = true;
            continue;
        }
        if(line != "\n" && printV){
            float xpos = (line.at(2)-'0')*0.1 + (line.at(3)-'0')*0.01 + (line.at(4)-'0')*0.001;
            float ypos = (line.at(8)-'0')*0.1 + (line.at(9)-'0')*0.01 + (line.at(10)-'0')*0.001;
            createVertexOnCanvas(QPoint(xpos*width(),ypos*height()));
            g->AllVertex.back()->hide();
            g->AllVertex.back()->isPrinted = true;
            g->AllVertex.back()->num = countVertex+1;
            if(filename == ":/text/src/HelloWorld.txt"){
                g->AllVertex.back()->info->name = "Hi!";
                g->AllVertex.back()->info->detail = "Hello world!";
            }
            else
                g->AllVertex.back()->info->detail = "";
            g->AllVertex.back()->info->updateInfo();
            g->AllVertex.back()->nameLabel->hide();
            g->AllVertex.back()->nameLabel->setText(g->AllVertex.back()->info->name);
            g->AllVertex.back()->nameLabel->adjustSize();
            g->AllVertex.back()->nameLabel->move(g->AllVertex.back()->relativePos.x()*width()- g->AllVertex.back()->nameLabel->width()/2, g->AllVertex.back()->relativePos.y()*height()+ g->AllVertex.back()->height());
            countVertex++;
        }
        if(line != "\n" && printE){
            int first = (line.at(0)-'0')*10 + line.at(1)-'0';
            int second = (line.at(3)-'0')*10 + line.at(4)-'0';
            g->CreateEdge(*(g->AllVertex.begin()+first-1),*(g->AllVertex.begin()+second-1),1,this);
            g->AllEdge.back()->afterPrint = false;
            update();
        }
    }
    countVertex = 0;
}

void MyCanvas::ReadVertexInfo(QString filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << file.errorString();
        return;
    }
    int count = 0;
    while(!file.atEnd()){
        QString line = file.readLine();
        (*(g->AllVertex.begin()+count))->info->name = line.section('\n',0,0);
        (*(g->AllVertex.begin()+count))->nameLabel->setText(line.section('\n',0,0));
        (*(g->AllVertex.begin()+count))->nameLabel->adjustSize();
        (*(g->AllVertex.begin()+count))->nameLabel->move((*(g->AllVertex.begin()+count))->relativePos.x()*width()- (*(g->AllVertex.begin()+count))->nameLabel->width()/2, (*(g->AllVertex.begin()+count))->relativePos.y()*height()+ (*(g->AllVertex.begin()+count))->height());
        while(line != "\n" && !file.atEnd()){
            line = file.readLine();
            (*(g->AllVertex.begin()+count))->info->detail.append(line);
        }
        (*(g->AllVertex.begin()+count))->info->updateInfo();
        count++;
    }
}

void MyCanvas::createVertexOnCanvas(QPoint pos)
{
    g->CreateVertex(pos,this);
    g->AllVertex.back()->info = new VertexInfo;
    QString num = QString::number(g->AllVertex.count());
    g->AllVertex.back()->info->name = "vertex"+num;
    g->AllVertex.back()->info->detail = "";
    g->AllVertex.back()->nameLabel->setText("vertex"+num);
    g->AllVertex.back()->nameLabel->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    QFont font;
    font.setFamily("Tahoma");
    font.setItalic(true);
    font.setBold(true);
    font.setPointSize(7*width()/1000);
    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor(253,120,0));
    g->AllVertex.back()->nameLabel->setPalette(pa);
    g->AllVertex.back()->nameLabel->setFont(font);
    g->AllVertex.back()->nameLabel->adjustSize();
    g->AllVertex.back()->nameLabel->move( g->AllVertex.back()->relativePos.x()*width()- g->AllVertex.back()->nameLabel->width()/2, g->AllVertex.back()->relativePos.y()*height()+ g->AllVertex.back()->height());
    g->AllVertex.back()->show();
    if(Widget::isShowingName)
         g->AllVertex.back()->nameLabel->show();
    else
        g->AllVertex.back()->nameLabel->hide();
    connect(g->AllVertex.back(),&Vertex::mouseDoubleClick,[this](Vertex *v){
        if(!startDraw){
            this->startVertex = v;
            this->startDraw = true;
            endPoint = startVertex->relativePos*width();
            qDebug() << "出发点坐标：" << startVertex->relativePos*width();
            qDebug() << "Start drawing!";
        }
    });
    connect(g->AllVertex.back(),&Vertex::mouseEnterHover,[this](Vertex *v){
        if(cursor().shape()!=Qt::PointingHandCursor)
            setCursor(QCursor(Qt::PointingHandCursor));
        if(startDraw && v != startVertex){
            this->endVertex = v;
            v->canExtendEdge = false;
            if(g->graphType == Graph::UNDIRECTED){
                endPoint.setX(v->relativePos.x()*this->size().width());
                endPoint.setY(v->relativePos.y()*this->size().height());
            }else{
                endPoint.setX(v->relativePos.x()*this->width()
                              -v->width()/2*cos(atan2((v->relativePos.y()*this->width() - startVertex->relativePos.y()*height())
                                                            , (v->relativePos.x()*this->width() - startVertex->relativePos.x()*width()))));
                endPoint.setY(v->relativePos.y()*this->width()
                              -v->height()/2*sin(atan2((v->relativePos.y()*this->width() - startVertex->relativePos.y()*height())
                                                             , (v->relativePos.x()*this->width() - startVertex->relativePos.x()*width()))));
            }
            update();
        }
    });
    connect(g->AllVertex.back(),&Vertex::mouseLeaveHover,[this](){
        if(cursor().shape()!=Qt::CrossCursor)
            setCursor(QCursor(Qt::CrossCursor));
        if(startDraw && endVertex){
            endVertex->canExtendEdge = true;
        }
    });
    connect(g->AllVertex.back(),&Vertex::lockEndVertex,[this](Vertex *v){
        if(startDraw){
            isFocused = true;
            endVertex = v;
            endVertex->canExtendEdge = true;
            qDebug() << "终点坐标：" << (endVertex->relativePos)*this->size().width();
        }
    });
    connect(g->AllVertex.back(),&Vertex::draggingSignal,[this](){
        if(cursor().shape()!=Qt::ClosedHandCursor)
            setCursor(QCursor(Qt::ClosedHandCursor));
        update();
    });
    connect(g->AllVertex.back(),&Vertex::deleteSignal,[this](Vertex *v){
        if(targetVertex == v){
            targetVertex->isTarget = false;
            targetVertex->updateSize();
            targetVertex = nullptr;
           }
        g->deleteVertex(v);
        if(cursor().shape()!=Qt::CrossCursor)
            setCursor(QCursor(Qt::CrossCursor));
        update();

    });
    connect(g->AllVertex.back(),&Vertex::lockTargetVertex,[this](Vertex *v){
        if(targetVertex){
            targetVertex->isTarget = false;
            targetVertex->updateSize();
        }
        targetVertex = v;
        emit getTargetVertex(v);
    });
}

void MyCanvas::showHelloWorld(QVariant value)
{
    if(value.toInt()<=1){
        (*(g->AllVertex.begin()+countVertex-1))->show();
        (*(g->AllVertex.begin()+countVertex-1))->blinkAnimation->start();
        if(Widget::isShowingName)
            (*(g->AllVertex.begin()+countVertex-1))->nameLabel->show();

    }else{
        if(countEdge<=38 && (*(g->AllEdge.begin()+countEdge-1))->vr->num<=countVertex+1){
            startPrintingPoint = (*(g->AllEdge.begin()+countEdge-1))->vl->relativePos*width();
            float x = (*(g->AllEdge.begin()+countEdge-1))->vr->relativePos.x()*width();
            float y = (*(g->AllEdge.begin()+countEdge-1))->vr->relativePos.y()*width();
            endPrintingPoint = startPrintingPoint + QPointF((x-startPrintingPoint.x())*(value.toFloat()-1)/10,(y-startPrintingPoint.y())*(value.toFloat()-1)/10);
            update();
            if(value.toInt() == 11)
                (*(g->AllEdge.begin()+countEdge-1))->afterPrint = true;
            edgeFlag = true;
        }else
            edgeFlag = false;
    }
}

void MyCanvas::showCoursesDiagram(QVariant value)
{
    if(value.toInt()<=1){
        (*(g->AllVertex.begin()+countVertex-1))->show();
        (*(g->AllVertex.begin()+countVertex-1))->blinkAnimation->start();
        if(Widget::isShowingName)
            (*(g->AllVertex.begin()+countVertex-1))->nameLabel->show();
    }else if(countEdge<=34 && (*(g->AllEdge.begin()+countEdge-1))->vr->num<=countVertex+1){
            int count = countEdge;
            while(count<=34 && (*(g->AllEdge.begin()+count-1))->vr->num<=countVertex+1){
                startPrintingPoint = (*(g->AllEdge.begin()+count-1))->vl->relativePos*width();
                float x = (*(g->AllEdge.begin()+count-1))->vr->relativePos.x()*width();
                float y = (*(g->AllEdge.begin()+count-1))->vr->relativePos.y()*width();
                endPrintingPoint = startPrintingPoint + QPointF((x-startPrintingPoint.x())*(value.toFloat()-1)/10,(y-startPrintingPoint.y())*(value.toFloat()-1)/10);
                update();
                if(value.toInt() == 11)
                    (*(g->AllEdge.begin()+count-1))->afterPrint = true;
                count++;
            }
            if(value.toInt() == 11) countEdge = count;
    }
}

void MyCanvas::showContent(QVariant value)
{
    if(value.toInt()<=1){
        (*(g->AllVertex.begin()+countVertex-1))->show();
        (*(g->AllVertex.begin()+countVertex-1))->blinkAnimation->start();
        if(Widget::isShowingName)
            (*(g->AllVertex.begin()+countVertex-1))->nameLabel->show();

    }else{
        if(countEdge <= 5 && (*(g->AllEdge.begin()+countEdge-1))->vr->num<=countVertex+1){
            startPrintingPoint = (*(g->AllEdge.begin()+countEdge-1))->vl->relativePos*width();
            float x = (*(g->AllEdge.begin()+countEdge-1))->vr->relativePos.x()*width();
            float y = (*(g->AllEdge.begin()+countEdge-1))->vr->relativePos.y()*width();
            endPrintingPoint = startPrintingPoint + QPointF((x-startPrintingPoint.x())*(value.toFloat()-1)/10,(y-startPrintingPoint.y())*(value.toFloat()-1)/10);
            update();
            if(value.toInt() == 11)
                (*(g->AllEdge.begin()+countEdge-1))->afterPrint = true;
            edgeFlag = true;
        }else
            edgeFlag = false;
    }
}


void MyCanvas::mouseMoveEvent(QMouseEvent *event)
{
    if(startDraw){
        endPoint = event->pos();
        update();
    }
    if(isDragging){
        move(event->globalPos() - offset);
        emit dragging();
    }

}

void MyCanvas::wheelEvent(QWheelEvent *event)
{
    if((width()<2000 && width()>50) || (width()<50 && event->angleDelta().y()>0) || (width()>2000 && event->angleDelta().y()<0)){
        resize(size().width()+event->angleDelta().y(),size().height()+event->angleDelta().y());
        for(QLinkedList<Vertex*>::iterator it = g->AllVertex.begin();it != g->AllVertex.end();it++){
            (*it)->info->hide();
            (*it)->showSize = 20 * width()/1000;
            (*it)->updateSize();
            (*it)->move((*it)->relativePos.x()*this->width()-(*it)->width()/2,(*it)->relativePos.y()*this->height()-(*it)->width()/2);
            QFont font = (*it)->nameLabel->font();
            font.setPointSize(7*(float)width()/1000);
            (*it)->nameLabel->setFont(font);
            (*it)->nameLabel->adjustSize();
            (*it)->nameLabel->move((*it)->relativePos.x()*width()-(*it)->nameLabel->width()/2,(*it)->relativePos.y()*height()+(*it)->height());
        }
        int penWidth;
        if(width()<200) penWidth = 1;
        else if (width()<800) penWidth = 2;
        else if (width()<1600) penWidth = 3;
        else penWidth = 4;
        pen1.setWidth(penWidth);
        pen2.setWidth(penWidth);
        l=20*(float)this->width()/1000;
        update();
        emit dragging();
    }
}
