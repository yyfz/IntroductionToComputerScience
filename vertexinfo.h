#ifndef VERTEXINFO_H
#define VERTEXINFO_H

#include <QWidget>

namespace Ui {
class VertexInfo;
}

class VertexInfo : public QWidget
{
    Q_OBJECT

public:
    explicit VertexInfo(QWidget *parent = nullptr);
    ~VertexInfo();
    QString name;
    void updateInfo();
    QString detail;
signals:
    void nameChange();
    void detailChang();

private:
    Ui::VertexInfo *ui;
};

#endif // VERTEXINFO_H
