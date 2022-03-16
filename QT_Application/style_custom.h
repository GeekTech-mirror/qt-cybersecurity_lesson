#ifndef STYLE_CUSTOM_H
#define STYLE_CUSTOM_H

#include <style_common.h>

#include <QWidget>
#include <QProxyStyle>
#include <QPalette>

class QStyleCustom : public QProxyStyle
{
    Q_OBJECT

public:
    QStyleCustom();
    ~QStyleCustom();

    void drawPrimitive(PrimitiveElement element, const QStyleOption *option,
                       QPainter *painter, const QWidget *widget) const override;

private:
    static QPainterPath roundRectPath(const QRect &rect);
    mutable QPalette m_standardPalette;
};

#endif // STYLE_CUSTOM_H
