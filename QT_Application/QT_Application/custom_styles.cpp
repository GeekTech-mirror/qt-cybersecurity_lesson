#include "custom_styles.h"

Custom_Button::Custom_Button (QStyle *parent)
{
    QPainter painter (this);
    painter.setBrush(QColor(0,0,0));
}

/*
Custom_Button::~Custom_Button ()
{
    delete ;
}
*/
void Custom_Button::paintEvent (QPaintEvent *parent)
{
    QStyleOption opt;
    opt.initFrom (this);

    QPainter p (this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
