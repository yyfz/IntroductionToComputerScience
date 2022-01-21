#include "vertexinfo.h"
#include "ui_vertexinfo.h"
#include <QGraphicsDropShadowEffect>
#include <QScrollBar>

VertexInfo::VertexInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VertexInfo)
{
    ui->setupUi(this);
    setWindowOpacity(0.8);
    setWindowFlag(Qt::FramelessWindowHint,true);
    QFont font;
    font.setFamily("Tahoma");
    font.setItalic(true);
    font.setPointSize(7);
    font.setBold(true);
    QPalette pa;
    pa.setColor(QPalette::WindowText,QColor(253,120,0));
    ui->nameTextLabel->setFont(font);
    ui->nameTextLabel->setPalette(pa);
    font.setFamily("仿宋");
    font.setItalic(false);
    font.setPointSize(6);
    ui->detailTextBrowser->setFont(font);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect;
    effect->setOffset(4,4);
    effect->setColor(QColor(0,0,0,50));
    effect->setBlurRadius(10);
    ui->frame->setGraphicsEffect(effect);
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
    ui->detailTextBrowser->verticalScrollBar()->setStyleSheet(scrollstyle);
}

VertexInfo::~VertexInfo()
{
    delete ui;
}

void VertexInfo::updateInfo()
{
    ui->nameTextLabel->setText(name);
    ui->detailTextBrowser->setText(detail);
}
