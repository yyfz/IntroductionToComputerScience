#include "content.h"
#include "ui_content.h"
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QScrollBar>

Content::Content(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Content)
{
    ui->setupUi(this);
    setWindowOpacity(0.8);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setOffset(4,4);
    effect->setColor(QColor(0,0,0,50));
    effect->setBlurRadius(10);
    ui->frame->setGraphicsEffect(effect);
    installEventFilter(this);
    ui->closeBtn->installEventFilter(this);
    ui->minimizeBtn->installEventFilter(this);
    ui->closeBtn->setAttribute(Qt::WA_Hover,true);
    ui->minimizeBtn->setAttribute(Qt::WA_Hover,true);
    enterAnimation = new QPropertyAnimation(this,"");
    enterAnimation->setStartValue(0);
    enterAnimation->setEndValue(30);
    enterAnimation->setDuration(100);
    connect(enterAnimation,&QPropertyAnimation::valueChanged,this,&Content::enterImageChanged);
    leaveAnimation = new QPropertyAnimation(this,"");
    leaveAnimation->setStartValue(0);
    leaveAnimation->setEndValue(30);
    leaveAnimation->setDuration(100);
    connect(leaveAnimation,&QPropertyAnimation::valueChanged,this,&Content::leaveImageChanged);
    ui->textBrowser->setOpenExternalLinks(true);
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
    ui->textBrowser->verticalScrollBar()->setStyleSheet(scrollstyle);
}

Content::~Content()
{
    delete ui;
}

void Content::setHtml(QString htmlFileName)
{
    ui->textBrowser->setSource(QUrl(htmlFileName));
}

void Content::enterImageChanged(QVariant value)
{
    if(targetBtn == ui->closeBtn){
        closeBtnSize = 20*(1+value.toFloat()/100);
        updateCloseBtnSize();
        ui->closeBtn->move(554-closeBtnSize/2,24-closeBtnSize/2);
    }else if(targetBtn == ui->minimizeBtn){
        minimizeBtnSize = 20*(1+value.toFloat()/100);
        updateMinimizeBtnSize();
        ui->minimizeBtn->move(514-minimizeBtnSize/2,24-minimizeBtnSize/2);
    }
}

void Content::leaveImageChanged(QVariant value)
{
    if(targetBtn == ui->closeBtn){
        if(value.toFloat() == 30)
            closeBtnSize = 20;
        else
            closeBtnSize = 20*(1.3-value.toFloat()/100);
        updateCloseBtnSize();
        ui->closeBtn->move(554-closeBtnSize/2,24-closeBtnSize/2);
    }else if (targetBtn == ui->minimizeBtn){
        if(value.toFloat() == 30)
            minimizeBtnSize = 20;
        else
            minimizeBtnSize = 20*(1.3-value.toFloat()/100);
        updateMinimizeBtnSize();
        ui->minimizeBtn->move(514-minimizeBtnSize/2,24-minimizeBtnSize/2);
    }
}

void Content::updateMinimizeBtnSize()
{
    ui->minimizeBtn->setStyleSheet("min-width: "+QString::number(minimizeBtnSize)+"; min-height: "+QString::number(minimizeBtnSize)+";max-width:"+QString::number(minimizeBtnSize)+"; max-height:"+QString::number(minimizeBtnSize)+"px;border-radius:"+QString::number(minimizeBtnSize/2)+";background-color: rgb(134, 255, 158);");
}

void Content::updateCloseBtnSize()
{
    ui->closeBtn->setStyleSheet("min-width: "+QString::number(closeBtnSize)+"; min-height: "+QString::number(closeBtnSize)+";max-width:"+QString::number(closeBtnSize)+"; max-height:"+QString::number(closeBtnSize)+"px;border-radius:"+QString::number(closeBtnSize/2)+";background-color: rgb(255, 123, 123);");
}

bool Content::eventFilter(QObject *obj, QEvent *event)
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
            if(ui->closeBtn->geometry().contains(ui->frame->mapFromGlobal(QCursor::pos())))
                close();
            return true;
        }
        if(event->type() == QEvent::HoverEnter){
            QApplication::setOverrideCursor(QCursor(Qt::CursorShape::PointingHandCursor));
            minimizeBtnSize = 20;
            updateMinimizeBtnSize();
            ui->minimizeBtn->move(514-minimizeBtnSize/2,24-minimizeBtnSize/2);
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
           if(ui->minimizeBtn->geometry().contains(ui->frame->mapFromGlobal(QCursor::pos())))
                showMinimized();
           return true;
       }if(event->type() == QEvent::HoverEnter){
           QApplication::setOverrideCursor(QCursor(Qt::CursorShape::PointingHandCursor));
           closeBtnSize = 20;
           updateCloseBtnSize();
           ui->closeBtn->move(554-minimizeBtnSize/2,24-minimizeBtnSize/2);
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
    }
    return false;
}

void Content::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton){
        if(dragging)
            move(event->globalPos() - offset);
    }
}

void Content::mouseReleaseEvent(QMouseEvent *event)
{
    QApplication::restoreOverrideCursor();
    if(dragging) dragging = false;
}
