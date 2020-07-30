#ifndef MYIMAGELABEL_H
#define MYIMAGELABEL_H

#include <QtWidgets>
#include <QLabel>

class MyImageLabel :public QLabel
{
    Q_OBJECT
public:
    explicit MyImageLabel(QWidget* parent = 0);
    void paintEvent(QPaintEvent* e);
    virtual ~MyImageLabel();
    const QImage* pCurrentImage;
private:
    QWidget* pParentWidget;
};

#endif // MYIMAGELABEL_H
