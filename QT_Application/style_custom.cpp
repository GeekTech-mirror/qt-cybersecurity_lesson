#include "style_custom.h"

#include <QPainter>
#include <QPushButton>
#include <QCommonStyle>
#include <QStyleOption>
#include <QStyleFactory>


enum Direction {
    TopDown,
    FromLeft,
    BottomUp,
    FromRight
};


static QColor merged_colors (const QColor &colorA,
                             const QColor &colorB,
                             int factor = 50)
{
    const int maxFactor = 100;
    QColor tmp = colorA;
    tmp.setRed ((tmp.red() * factor) /
                maxFactor + (colorB.red() * (maxFactor - factor)) / maxFactor);
    tmp.setGreen ((tmp.green() * factor) /
                  maxFactor + (colorB.green() * (maxFactor - factor)) / maxFactor);
    tmp.setBlue ((tmp.blue() * factor) /
                 maxFactor + (colorB.blue() * (maxFactor - factor)) / maxFactor);
    return tmp;
}


// The default button and handle gradient
static QLinearGradient qt_fusion_gradient (const QRect &rect,
                                           const QBrush &baseColor,
                                           Direction direction = TopDown)
{
    int x = rect.center().x();
    int y = rect.center().y();
    QLinearGradient gradient;
    switch (direction) {
    case FromLeft:
        gradient = QLinearGradient(rect.left(), y, rect.right(), y);
        break;
    case FromRight:
        gradient = QLinearGradient(rect.right(), y, rect.left(), y);
        break;
    case BottomUp:
        gradient = QLinearGradient(x, rect.bottom(), x, rect.top());
        break;
    case TopDown:
    default:
        gradient = QLinearGradient(x, rect.top(), x, rect.bottom());
        break;
    }
    if (baseColor.gradient())
        gradient.setStops(baseColor.gradient()->stops());
    else {
        QColor gradientStartColor = baseColor.color().lighter(124);
        QColor gradientStopColor = baseColor.color().lighter(102);
        gradient.setColorAt(0, gradientStartColor);
        gradient.setColorAt(1, gradientStopColor);
        //          Uncomment for adding shiny shading
        //            QColor midColor1 = merged_colors(gradientStartColor, gradientStopColor, 55);
        //            QColor midColor2 = merged_colors(gradientStartColor, gradientStopColor, 45);
        //            gradient.setColorAt(0.5, midColor1);
        //            gradient.setColorAt(0.501, midColor2);
    }
    return gradient;
}


QStyleCustom::QStyleCustom () :
    QProxyStyle (QStyleFactory::create("fusion"))
{
    setObjectName ("Custom");
}


QStyleCustom::~QStyleCustom ()
{
}


void QStyleCustom::drawPrimitive (PrimitiveElement element,
                                  const QStyleOption *option,
                                  QPainter *painter,
                                  const QWidget *widget) const
{
    QStyleCommon ColorPalette;

    QColor button_color = QColor(40,44,52);
    QColor outline = ColorPalette.outline(option->palette);
    QColor highlighted_outline = ColorPalette.highlighted_outline(option->palette);

    int button_brightness = 125;
    int corner_radius = 16;

    switch (element) {
    case PE_PanelButtonCommand: {
        bool isDefault = false;
        bool isDown = (option->state & State_Sunken) || (option->state & State_On);
        QRect button_shape;
        QRect rect = option->rect;

        if (const QStyleOptionButton *button =
                qstyleoption_cast<const QStyleOptionButton*>(option)) {
            isDefault =
                (button->features & QStyleOptionButton::DefaultButton) &&
                (button->state & State_Enabled);
        }

        bool isEnabled = option->state & State_Enabled;
        bool hasFocus =
            (option->state & State_HasFocus &&
             option->state & State_KeyboardFocusChange);

        button_color = button_color.lighter(button_brightness);

        QColor dark_outline = outline;
        if (hasFocus | isDefault) {
            dark_outline = highlighted_outline;
        }

        if (isDefault)
            button_color = merged_colors (button_color, highlighted_outline.lighter(130), 90);

        QPainter *p = painter;
        button_shape = rect.adjusted (0, 1, -1, 0);

        p->setRenderHint (QPainter::Antialiasing, true);
        p->translate (0.5, -0.5);

        QLinearGradient gradient =
            qt_fusion_gradient (rect, (isEnabled && option->state & State_MouseOver)
                                ? button_color : button_color.darker (104) );

        p->setPen(Qt::transparent);
        p->setBrush(isDown ? QBrush(button_color.darker(110)) : gradient);
        p->drawRoundedRect(button_shape, corner_radius, corner_radius);
        p->setBrush(Qt::NoBrush);

        // Outline
        p->setPen(!isEnabled ? QPen(dark_outline.lighter(115)) : QPen(dark_outline));
        p->drawRoundedRect(button_shape, corner_radius, corner_radius);

        p->setPen(QColor(255, 255, 255, 30));
        p->drawRoundedRect(button_shape.adjusted(1, 1, -1, -1), corner_radius, corner_radius);

        break;
        }
    }
}

