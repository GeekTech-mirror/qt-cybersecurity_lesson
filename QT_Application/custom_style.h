#ifndef STYLE_CUSTOM_H
#define STYLE_CUSTOM_H

#include <QAbstractScrollArea>
#include <QProxyStyle>
#include <QStyleOption>

class CustomStylePrivate;
class CustomStyle : public QProxyStyle
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(CustomStyle)

public:
    CustomStyle();
    ~CustomStyle();

    void polishScrollArea( QAbstractScrollArea* );

    void drawPrimitive (PrimitiveElement element,
                        const QStyleOption *option,
                        QPainter *painter,
                        const QWidget *widget) const override;

    void drawControl (ControlElement element,
                      const QStyleOption *option,
                      QPainter *painter,
                      const QWidget *widget) const override;


protected:
    CustomStyle(CustomStylePrivate &dd);
    CustomStylePrivate *d_ptr;
};

#endif // STYLE_CUSTOM_H
