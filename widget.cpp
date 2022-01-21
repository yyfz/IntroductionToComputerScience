#include "widget.h"
#include "ui_widget.h"
#include <QBitmap>
#include <QPropertyAnimation>
#include <QDebug>
#include <QPainter>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QScrollBar>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    targetVertex = nullptr;
    isShowingName = false;
    canvas = new MyCanvas(ui->frame);
    connect(canvas,&MyCanvas::dragging,[this](){update();});
    ui->vertexNameLineEdit->setAlignment(Qt::AlignCenter);
    ui->label->hide();
    ui->label_2->hide();
    ui->vertexDetailTextEdit->hide();
    ui->vertexNameLineEdit->hide();
    ui->showNameBtn->setFont(QFont("HGDY_CNKI",9));
    ui->clearBtn->setFont(QFont("HGDY_CNKI",9));
    ui->showDiagramBtn->setFont(QFont("HGDY_CNKI",9));
    ui->showMoreBtn->setFont(QFont("HGDY_CNKI",9));
    connect(canvas,&MyCanvas::getTargetVertex,[this](Vertex *v){\
        ui->HelloWorld->hide();
        ui->tipLabel->hide();
        this->targetVertex = v;
        ui->label->show();
        ui->label_2->show();
        ui->vertexDetailTextEdit->show();
        ui->vertexNameLineEdit->show();
        if(targetVertex->info){
            ui->vertexNameLineEdit->setText(targetVertex->info->name);
            ui->vertexDetailTextEdit->setText(targetVertex->info->detail);
        }
        canvas->update();
    });
    connect(canvas,&MyCanvas::loseTargetVertex,[this](){
        ui->HelloWorld->show();
        ui->tipLabel->show();
        targetVertex->isTarget = false;
        targetVertex->updateSize();
        targetVertex = nullptr;
        ui->label->hide();
        ui->label_2->hide();
        ui->vertexDetailTextEdit->hide();
        ui->vertexNameLineEdit->hide();
    });
    timer1 = startTimer(700);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowOpacity(0.95);
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::transparent);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(), 20, 20);
    setMask(bmp);
    setFixedSize(size());
    installEventFilter(this);
    ui->closeBtn->installEventFilter(this);
    ui->minimizeBtn->installEventFilter(this);
    ui->showNameBtn->installEventFilter(this);
    ui->clearBtn->installEventFilter(this);
    ui->showDiagramBtn->installEventFilter(this);
    ui->showMoreBtn->installEventFilter(this);
    ui->closeBtn->setAttribute(Qt::WA_Hover,true);
    ui->minimizeBtn->setAttribute(Qt::WA_Hover,true);
    ui->showDiagramBtn->setAttribute(Qt::WA_Hover,true);
    ui->showMoreBtn->setAttribute(Qt::WA_Hover,true);
    enterAnimation = new QPropertyAnimation(this,"");
    enterAnimation->setStartValue(0);
    enterAnimation->setEndValue(30);
    enterAnimation->setDuration(100);
    connect(enterAnimation,&QPropertyAnimation::valueChanged,this,&Widget::enterImageChanged);
    leaveAnimation = new QPropertyAnimation(this,"");
    leaveAnimation->setStartValue(0);
    leaveAnimation->setEndValue(30);
    leaveAnimation->setDuration(100);
    connect(leaveAnimation,&QPropertyAnimation::valueChanged,this,&Widget::leaveImageChanged);
    QString scrollstyle="QScrollBar:vertical"
                        "{"
                        "width:8px;"
                        "background:transparent;"
                        "margin:0px,0px,0px,0px;"
                        "padding-top:0px;"
                        "padding-bottom:0px;"
                        "}"
                        "QScrollBar::handle:vertical"
                        "{"
                        "width:8px;"
                        "background:rgba(0,0,0,50%);"
                        "border-radius:4px;"
                        "min-height:80"
                        "}"
                        "QScrollBar::handle:vertical:hover"
                        "{"
                        "width:8px;"
                        "background:rgba(0,0,0,50%);"
                        " border-radius:4px;"
                        "min-height:80;"
                        "}"
                        "QScrollBar::add-line:vertical"
                        "{"
                        "height:0px;width:8px;"
                        "subcontrol-position:bottom;"
                        "}"
                        "QScrollBar::sub-line:vertical"
                        "{"
                        "height:0px;width:8px;"
                        "subcontrol-position:top;"
                        "}"
                        "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
                        "{"
                        "background:transparent;"
                        "border-radius:4px;"
                        "}";
    ui->vertexDetailTextEdit->verticalScrollBar()->setStyleSheet(scrollstyle);
}


Widget::~Widget()
{
    delete ui;
}

void Widget::on_showNameBtn_toggled(bool checked)
{
    if(checked){
        isShowingName = true;
        ui->showNameBtn->setText("hide vertex name");
        for(QLinkedList<Vertex*>::const_iterator it = canvas->g->AllVertex.begin(); it != canvas->g->AllVertex.end();it++){
            if(canvas->isPrinting){
                if((*it)->num <= canvas->countVertex)
                    (*it)->nameLabel->show();
                else if((*it)->num == 0)
                    (*it)->nameLabel->show();
            }else
                (*it)->nameLabel->show();
        }
        canvas->update();
    }else {
        isShowingName = false;
        ui->showNameBtn->setText("show vertex name");
        for(QLinkedList<Vertex*>::const_iterator it = canvas->g->AllVertex.begin(); it != canvas->g->AllVertex.end();it++){
                (*it)->nameLabel->hide();
        }
    }
}

void Widget::on_clearBtn_clicked()
{
    if(!canvas->isPrinting){
        if(targetVertex){
            targetVertex = nullptr;
            ui->HelloWorld->show();
            ui->tipLabel->show();
            ui->label->hide();
            ui->label_2->hide();
            ui->vertexDetailTextEdit->hide();
            ui->vertexNameLineEdit->hide();
        }
        canvas->g->clear();
        canvas->clear();
        canvas->update();
    }
}

void Widget::on_vertexNameLineEdit_textChanged(const QString &arg1)
{
    if(targetVertex->info){
        targetVertex->info->name = arg1;
        targetVertex->info->updateInfo();
        targetVertex->nameLabel->setText(arg1);
        targetVertex->nameLabel->adjustSize();
        targetVertex->nameLabel->move(targetVertex->relativePos.x()*canvas->width()- targetVertex->nameLabel->width()/2, targetVertex->relativePos.y()*canvas->height()+ targetVertex->height());
    }
}

void Widget::on_vertexDetailTextEdit_textChanged()
{
    if(targetVertex->info){
        targetVertex->info->detail = ui->vertexDetailTextEdit->toPlainText();
        targetVertex->info->updateInfo();
    }
}

bool Widget::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == this){
        if(event->type() == QEvent::MouseButtonPress){
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() == Qt::LeftButton){
                dragging = true;
                offset = mouseEvent->globalPos() - pos();
                QApplication::setOverrideCursor(QCursor(Qt::CursorShape::ClosedHandCursor));
            }
            return true;
        }return false;
    }
    if(obj == ui->closeBtn){
        if(event->type() == QEvent::MouseButtonPress){
            dragging = false;
            return true;
        }
        if(event->type() == QEvent::MouseButtonRelease){
            if(ui->closeBtn->geometry().contains(this->mapFromGlobal(QCursor::pos())))
                close();
            return true;
        }
        if(event->type() == QEvent::HoverEnter){
            QApplication::setOverrideCursor(QCursor(Qt::CursorShape::PointingHandCursor));
            minimizeBtnSize = 20;
            updateMinimizeBtnSize();
            ui->minimizeBtn->move(1140-minimizeBtnSize/2,20-minimizeBtnSize/2);
            targetBtn = ui->closeBtn;
            enterAnimation->start();
            return true;
        }
        if(event->type() == QEvent::HoverLeave){
            QApplication::restoreOverrideCursor();
            leaveAnimation->start();
            return true;
        }
        return false;
    }else if(obj == ui->minimizeBtn){
        if(event->type() == QEvent::MouseButtonPress){
            dragging = false;
            return true;
        }
       if(event->type() ==QEvent::MouseButtonRelease){
           if(ui->minimizeBtn->geometry().contains(this->mapFromGlobal(QCursor::pos())))
                showMinimized();
           return true;
       }if(event->type() == QEvent::HoverEnter){
           QApplication::setOverrideCursor(QCursor(Qt::CursorShape::PointingHandCursor));
           closeBtnSize = 20;
           updateCloseBtnSize();
           ui->closeBtn->move(1180-minimizeBtnSize/2,20-minimizeBtnSize/2);
           targetBtn = ui->minimizeBtn;
           enterAnimation->start();
           return true;
       }
       if(event->type() == QEvent::HoverLeave){
           QApplication::restoreOverrideCursor();
           leaveAnimation->start();
           return true;
       }
       return false;
    }else if(obj == ui->showNameBtn){
        if(event->type() == QEvent::HoverEnter){
            QApplication::setOverrideCursor(QCursor(Qt::CursorShape::PointingHandCursor));
            ui->clearBtn->resize(130,40);
            ui->clearBtn->move(895,670);
            ui->clearBtn->setFont(QFont("HGDY_CNKI",9));
            ui->showDiagramBtn->resize(280,40);
            ui->showDiagramBtn->move(820,490);
            ui->showDiagramBtn->setFont(QFont("HGDY_CNKI",9));
            ui->showMoreBtn->resize(170,40);
            ui->showMoreBtn->move(875,610);
            ui->showMoreBtn->setFont(QFont("HGDY_CNKI",9));
            targetBtn = ui->showNameBtn;
            enterAnimation->start();
            return true;
        }else if(event->type() == QEvent::HoverLeave){
            QApplication::restoreOverrideCursor();
            targetBtn = ui->showNameBtn;
            leaveAnimation->start();
            return true;
        }
        return false;
    }else if(obj == ui->clearBtn){
        if(event->type() == QEvent::HoverEnter){
            QApplication::setOverrideCursor(QCursor(Qt::CursorShape::PointingHandCursor));
            ui->showNameBtn->resize(230,40);
            ui->showNameBtn->move(845,550);
            ui->showNameBtn->setFont(QFont("HGDY_CNKI",9));
            ui->showDiagramBtn->resize(280,40);
            ui->showDiagramBtn->move(820,490);
            ui->showDiagramBtn->setFont(QFont("HGDY_CNKI",9));
            ui->showMoreBtn->resize(170,40);
            ui->showMoreBtn->move(875,610);
            ui->showMoreBtn->setFont(QFont("HGDY_CNKI",9));
            targetBtn = ui->clearBtn;
            enterAnimation->start();
            return true;
        }else if(event->type() == QEvent::HoverLeave){
            QApplication::restoreOverrideCursor();
            targetBtn = ui->clearBtn;
            leaveAnimation->start();
            return true;
        }
        return false;
    }else if(obj == ui->showDiagramBtn){
        if(event->type() == QEvent::HoverEnter){
            QApplication::setOverrideCursor(QCursor(Qt::CursorShape::PointingHandCursor));
            ui->showNameBtn->resize(230,40);
            ui->showNameBtn->move(845,550);
            ui->showNameBtn->setFont(QFont("HGDY_CNKI",9));
            ui->clearBtn->resize(130,40);
            ui->clearBtn->move(895,670);
            ui->clearBtn->setFont(QFont("HGDY_CNKI",9));
            ui->showMoreBtn->resize(170,40);
            ui->showMoreBtn->move(875,610);
            ui->showMoreBtn->setFont(QFont("HGDY_CNKI",9));
            targetBtn = ui->showDiagramBtn;
            enterAnimation->start();
            return true;
        }else if(event->type() == QEvent::HoverLeave){
            QApplication::restoreOverrideCursor();
            targetBtn = ui->showDiagramBtn;
            leaveAnimation->start();
            return true;
        }
        return false;
    }else if(obj == ui->showMoreBtn){
        if(event->type() == QEvent::HoverEnter){
            QApplication::setOverrideCursor(QCursor(Qt::CursorShape::PointingHandCursor));
            ui->showNameBtn->resize(230,40);
            ui->showNameBtn->move(845,550);
            ui->showNameBtn->setFont(QFont("HGDY_CNKI",9));
            ui->clearBtn->resize(130,40);
            ui->clearBtn->move(895,670);
            ui->clearBtn->setFont(QFont("HGDY_CNKI",9));
            ui->showDiagramBtn->resize(280,40);
            ui->showDiagramBtn->move(820,490);
            ui->showDiagramBtn->setFont(QFont("HGDY_CNKI",9));
            targetBtn = ui->showMoreBtn;
            enterAnimation->start();
            return true;
        }else if(event->type() == QEvent::HoverLeave){
            QApplication::restoreOverrideCursor();
            targetBtn = ui->showMoreBtn;
            leaveAnimation->start();
            return true;
        }
        return false;
    }
     return false;
}

void Widget::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == timer1){
        timer2 = startTimer(250);
        canvas->isPrinting = true;
        canvas->ReadFile(":/text/src/HelloWorld.txt");
        timer1 = 0;
    }
    if(event->timerId() == timer2){
        if(canvas->countVertex < 44){
            canvas->countVertex++;
            if(canvas->edgeFlag) canvas->countEdge++;
            canvas->printHelloWorldAnimation->start();
        }else{
            timer2 = 0;
            canvas->countVertex = 0;
            canvas->countEdge = 0;
            canvas->startPrintingPoint = QPoint(0,0);
            canvas->endPrintingPoint = QPoint(0,0);
            canvas->edgeFlag = true;
            for(QLinkedList<Vertex*>::iterator it = canvas->g->AllVertex.begin();it != canvas->g->AllVertex.end(); it++)
                (*it)->isPrinted = false;
            timer4 = startTimer(600);
        }
    }else if(event->timerId() == timer3){
        if(canvas->countVertex < 26){
            canvas->countVertex++;
            canvas->printCoursesDiagramAnimation->start();
        }else{
            timer3 = 0;
            canvas->countVertex = 0;
            canvas->countEdge = 0;
            canvas->startPrintingPoint = QPoint(0,0);
            canvas->endPrintingPoint = QPoint(0,0);
            canvas->update();
            for(QLinkedList<Vertex*>::iterator it = canvas->g->AllVertex.begin();it != canvas->g->AllVertex.end(); it++)
                (*it)->isPrinted = false;
            timer4 = startTimer(600);
        }
    }else if(event->timerId() == timer4){
        timer4 = 0;
        canvas->isPrinting = false;
    }else if(event->timerId() == timer5){
        if(canvas->countVertex < 5){
            canvas->countVertex++;
            if(canvas->edgeFlag) canvas->countEdge++;
            canvas->printContentAnimation->start();
        }else{
            timer5 = 0;
            canvas->countVertex = 0;
            canvas->countEdge = 0;
            canvas->edgeFlag = true;
            canvas->startPrintingPoint = QPoint(0,0);
            canvas->endPrintingPoint = QPoint(0,0);
            canvas->update();
            for(QLinkedList<Vertex*>::iterator it = canvas->g->AllVertex.begin();it != canvas->g->AllVertex.end(); it++){
                (*it)->isPrinted = false;
                (*it)->num = -1;
            }
            timer4 = startTimer(600);
        }
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton){
        if(dragging)
            move(event->globalPos() - offset);
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    QApplication::restoreOverrideCursor();
    if(dragging) dragging = false;
}

void Widget::enterImageChanged(QVariant value)
{
    if(targetBtn == ui->closeBtn){
        closeBtnSize = 20*(1+value.toFloat()/100);
        updateCloseBtnSize();
        ui->closeBtn->move(1180-closeBtnSize/2,20-closeBtnSize/2);
    }else if(targetBtn == ui->minimizeBtn){
        minimizeBtnSize = 20*(1+value.toFloat()/100);
        updateMinimizeBtnSize();
        ui->minimizeBtn->move(1140-minimizeBtnSize/2,20-minimizeBtnSize/2);
    }else if(targetBtn == ui->showNameBtn){
        targetBtn->resize(230*(1+value.toFloat()/100),40*(1+value.toFloat()/100));
        targetBtn->setFont(QFont("HGDY_CNKI",9*(1+value.toFloat()/100)));
        targetBtn->move(960-targetBtn->width()/2,570-targetBtn->height()/2);
    }else if(targetBtn == ui->clearBtn){
        targetBtn->resize(130*(1+value.toFloat()/100),40*(1+value.toFloat()/100));
        targetBtn->setFont(QFont("HGDY_CNKI",9*(1+value.toFloat()/100)));
        targetBtn->move(960-targetBtn->width()/2,690-targetBtn->height()/2);
    }else if(targetBtn == ui->showDiagramBtn){
        targetBtn->resize(280*(1+value.toFloat()/100),40*(1+value.toFloat()/100));
        targetBtn->setFont(QFont("HGDY_CNKI",9*(1+value.toFloat()/100)));
        targetBtn->move(960-targetBtn->width()/2,510-targetBtn->height()/2);
    }else if(targetBtn == ui->showMoreBtn){
        targetBtn->resize(170*(1+value.toFloat()/100),40*(1+value.toFloat()/100));
        targetBtn->setFont(QFont("HGDY_CNKI",9*(1+value.toFloat()/100)));
        targetBtn->move(960-targetBtn->width()/2,630-targetBtn->height()/2);
    }
}

void Widget::leaveImageChanged(QVariant value)
{
    if(targetBtn == ui->closeBtn){
        if(value.toFloat() == 30)
            closeBtnSize = 20;
        else
            closeBtnSize = 20*(1.3-value.toFloat()/100);
        updateCloseBtnSize();
        ui->closeBtn->move(1180-closeBtnSize/2,20-closeBtnSize/2);
    }else if (targetBtn == ui->minimizeBtn){
        if(value.toFloat() == 30)
            minimizeBtnSize = 20;
        else
            minimizeBtnSize = 20*(1.3-value.toFloat()/100);
        updateMinimizeBtnSize();
        ui->minimizeBtn->move(1140-minimizeBtnSize/2,20-minimizeBtnSize/2);
    }else if(targetBtn == ui->showNameBtn){
        if(value.toFloat() == 30){
            targetBtn->resize(230,40);
            targetBtn->setFont(QFont("HGDY_CNKI",9));
        }
        else{
            targetBtn->resize(230*(1.3-value.toFloat()/100),40*(1.3-value.toFloat()/100));
            targetBtn->setFont(QFont("HGDY_CNKI",9*(1.3-value.toFloat()/100)));
        }
        targetBtn->move(960-targetBtn->width()/2,570-targetBtn->height()/2);
    }else if(targetBtn == ui->clearBtn){
        if(value.toFloat() == 30){
            targetBtn->resize(130,40);
            targetBtn->setFont(QFont("HGDY_CNKI",9));
        }
        else{
            targetBtn->resize(130*(1.3-value.toFloat()/100),40*(1.3-value.toFloat()/100));
            targetBtn->setFont(QFont("HGDY_CNKI",9*(1.3-value.toFloat()/100)));
        }
        targetBtn->move(960-targetBtn->width()/2,690-targetBtn->height()/2);
    }else if(targetBtn == ui->showDiagramBtn){
        if(value.toFloat() == 30){
            targetBtn->resize(280,40);
            targetBtn->setFont(QFont("HGDY_CNKI",9));
        }
        else{
            targetBtn->resize(280*(1.3-value.toFloat()/100),40*(1.3-value.toFloat()/100));
            targetBtn->setFont(QFont("HGDY_CNKI",9*(1.3-value.toFloat()/100)));
        }
        targetBtn->move(960-targetBtn->width()/2,510-targetBtn->height()/2);
    }else if(targetBtn == ui->showMoreBtn){
        if(value.toFloat() == 30){
            targetBtn->resize(170,40);
            targetBtn->setFont(QFont("HGDY_CNKI",9));
        }
        else{
            targetBtn->resize(170*(1.3-value.toFloat()/100),40*(1.3-value.toFloat()/100));
            targetBtn->setFont(QFont("HGDY_CNKI",9*(1.3-value.toFloat()/100)));
        }
        targetBtn->move(960-targetBtn->width()/2,630-targetBtn->height()/2);
    }
}

void Widget::updateMinimizeBtnSize()
{
    ui->minimizeBtn->setStyleSheet("min-width: "+QString::number(minimizeBtnSize)+"; min-height: "+QString::number(minimizeBtnSize)+";max-width:"+QString::number(minimizeBtnSize)+"; max-height:"+QString::number(minimizeBtnSize)+"px;border-radius:"+QString::number(minimizeBtnSize/2)+";background-color: rgb(134, 255, 158);");
}

void Widget::updateCloseBtnSize()
{
    ui->closeBtn->setStyleSheet("min-width: "+QString::number(closeBtnSize)+"; min-height: "+QString::number(closeBtnSize)+";max-width:"+QString::number(closeBtnSize)+"; max-height:"+QString::number(closeBtnSize)+"px;border-radius:"+QString::number(closeBtnSize/2)+";background-color: rgb(255, 123, 123);");
}

void Widget::on_showDiagramBtn_clicked()
{
    if(!canvas->isPrinting){
        canvas->isPrinting = true;
        if(targetVertex){
            targetVertex = nullptr;
            ui->HelloWorld->show();
            ui->tipLabel->show();
            ui->label->hide();
            ui->label_2->hide();
            ui->vertexDetailTextEdit->hide();
            ui->vertexNameLineEdit->hide();
        }
        canvas->g->clear();
        canvas->clear();
        canvas->update();
        canvas->ReadFile(":/text/src/CoursesDiagram.txt");
        canvas->ReadVertexInfo(":/text/src/CoursesDiagramVertexInfo.txt");
        canvas->g->graphType = Graph::DIRECTED;
        ui->showNameBtn->setChecked(true);
        canvas->countEdge++;
        timer3 = startTimer(250);
    }
}

void Widget::on_showMoreBtn_clicked()
{
    if(!canvas->isPrinting){
        canvas->isPrinting = true;
        if(targetVertex){
            targetVertex = nullptr;
            ui->HelloWorld->show();
            ui->tipLabel->show();
            ui->label->hide();
            ui->label_2->hide();
            ui->vertexDetailTextEdit->hide();
            ui->vertexNameLineEdit->hide();
        }
        canvas->g->clear();
        canvas->clear();
        canvas->update();
        canvas->ReadFile(":/text/src/Content.txt");
        canvas->ReadVertexInfo(":/text/src/ContentVertexInfo.txt");
        (*(canvas->g->AllVertex.begin()))->content = new Content;
        (*(canvas->g->AllVertex.begin()))->content->setHtml("qrc:/text/src/Website.html");

        (*(canvas->g->AllVertex.begin()+1))->content = new Content;
        (*(canvas->g->AllVertex.begin()+1))->content->setHtml("qrc:/text/src/Book.html");

        (*(canvas->g->AllVertex.begin()+2))->content = new Content;
        (*(canvas->g->AllVertex.begin()+2))->content->setHtml("qrc:/text/src/Favorites.html");

        (*(canvas->g->AllVertex.begin()+3))->content = new Content;
        (*(canvas->g->AllVertex.begin()+3))->content->setHtml("qrc:/text/src/Recognition.html");

        (*(canvas->g->AllVertex.begin()+4))->content = new Content;
        (*(canvas->g->AllVertex.begin()+4))->content->setHtml("qrc:/text/src/Plan.html");

        canvas->g->graphType = Graph::DIRECTED;
        ui->showNameBtn->setChecked(true);
        timer5 = startTimer(250);
    }
}
