#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include "graph.h"
#include "mycanvas.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    static bool isShowingName;

private slots:
    void on_showNameBtn_toggled(bool checked);
    void on_clearBtn_clicked();

    void on_vertexNameLineEdit_textChanged(const QString &arg1);

    void on_vertexDetailTextEdit_textChanged();

    void on_showDiagramBtn_clicked();

    void on_showMoreBtn_clicked();

signals:
protected:
    void timerEvent(QTimerEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    Ui::Widget *ui;
    MyCanvas *canvas;
    Vertex *targetVertex;
    int timer1 = 0;
    int timer2 = 0;
    int timer3 = 0;
    int timer4 = 0;
    int timer5 = 0;
    QPropertyAnimation *enterAnimation;
    QPropertyAnimation *leaveAnimation;
    void enterImageChanged(QVariant value);
    void leaveImageChanged(QVariant value);
    int closeBtnSize = 20;
    int minimizeBtnSize = 20;
    void updateMinimizeBtnSize();
    void updateCloseBtnSize();
    QWidget *targetBtn;
    QPoint offset;
    bool dragging = false;
    bool eventFilter(QObject *obj,QEvent *event);
};
#endif // WIDGET_H
