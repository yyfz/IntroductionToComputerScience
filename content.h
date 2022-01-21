#ifndef CONTENT_H
#define CONTENT_H

#include <QWidget>
#include <QPropertyAnimation>

namespace Ui {
class Content;
}

class Content : public QWidget
{
    Q_OBJECT

public:
    explicit Content(QWidget *parent = nullptr);
    ~Content();
    void setHtml(QString htmlFileName);

private:
    Ui::Content *ui;
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
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // CONTENT_H
