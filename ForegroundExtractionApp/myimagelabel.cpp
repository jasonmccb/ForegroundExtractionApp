#include "myimagelabel.h"

MyImageLabel::MyImageLabel(QWidget * parent): QLabel(parent)
{
	pParentWidget = parent;
	pCurrentImage = nullptr;
}

void MyImageLabel::paintEvent(QPaintEvent *e)
{
	int w = pParentWidget->width() - 50;
	if (w <= 0) w = 1;
	int h = pParentWidget->height() - 100;
	if (h <= 0) h = 1;

	if (!pCurrentImage || pCurrentImage->width()<=0 || pCurrentImage->height()<=0)
		return;

	double wscale = w * 1.0 / pCurrentImage->width();
	double hscale = h * 1.0 / pCurrentImage->height();
	double scale = wscale < hscale ? wscale : hscale;
	if (scale > 1) scale = 1;

	QPainter painter;
	painter.begin(this);
	painter.scale(scale, scale);
	painter.drawImage(10, 10, *pCurrentImage);
	painter.end();

	

}

MyImageLabel::~MyImageLabel() {}
