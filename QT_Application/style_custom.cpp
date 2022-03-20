/* Custom Application Style
** File: style_custom.cpp
** --------
** Make changes to the default "Fusion" Style
** --------
**
** Features:
** - Add Rounded corners on button elements
**   * corner_radius: controls roundness of corners
**
** - Change button color
**   * background_color: controls button's base color
**   * button_brightness: change button color brightness
*/

/* QT header files */
#include <QPainter>
#include <QPushButton>
#include <QStyleOption>
#include <QStyleFactory>

/* local includes */
#include "style_custom.h"
#include "style_custom_p.h"


enum Direction {
    TopDown,
    FromLeft,
    BottomUp,
    FromRight
};


/* Merge two colors */
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


/* The default button and handle gradient */
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


/* Constructor */
QStyleCustom::QStyleCustom () :
    QProxyStyle (QStyleFactory::create("fusion"))
{
    setObjectName ("custom");
}

/* Private Contructor */
QStyleCustom::QStyleCustom (QStyleCustomPrivate &dd) : d_ptr(&dd)
{
}

/* Destructor */
QStyleCustom::~QStyleCustom ()
{
}


/* Define visual representation of UI elements
** Parameters:
**     primitive element - specify which UI element to draw
**     style options - specify how and where to render the element
**     painter - paint handler to draw the element
**     widget (optional) - specify a widget to draw on
** Return: none
** Notes: re-implementation of fusion style
*/
void QStyleCustom::drawPrimitive (PrimitiveElement element,
                                  const QStyleOption *option,
                                  QPainter *painter,
                                  const QWidget *widget) const
{
    // Access color options from private data pointer
    QColor background_color = QColor(40,44,52);
    QColor outline = d_ptr->outline(option->palette);
    QColor highlighted_outline = d_ptr->highlighted_outline(option->palette);

    // Initialize button shape
    QRect button_shape;
    QRect rect = option->rect;

    int button_brightness = 125;
    int corner_radius = qMin(rect.width(), rect.height()) / 6;

    // Primitive element selector
    switch (element) {
    case PE_PanelButtonCommand: {

        // Increase color brightness
        QColor button_color = background_color.lighter(button_brightness);

        // Check button state
        bool isDefault = false;
        bool isDown = (option->state & State_Sunken) || (option->state & State_On);

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

        // Set highlight color for selected (default) button
        QColor dark_outline = outline;
        if (hasFocus | isDefault) {
            dark_outline = highlighted_outline;
        }

        if (isDefault)
            button_color = merged_colors (button_color,
                                          highlighted_outline.lighter(130),
                                          90);

        // Set button location
        QPainter *p = painter;
        button_shape = rect.adjusted (0, 1, -1, 0);

        // Use Anti-aliasing and corner center
        p->setRenderHint (QPainter::Antialiasing, true);
        p->translate (0.5, -0.5);

        // Apply subtle gradient from top to bottum
        QLinearGradient gradient =
            qt_fusion_gradient (rect, (isEnabled && option->state & State_MouseOver)
                                ? button_color : button_color.darker (104));

        // Paint button color based on whether button's pressed
        p->setPen (Qt::transparent);
        p->setBrush (isDown ? QBrush(button_color.darker(110)) : gradient);
        p->drawRoundedRect (button_shape, corner_radius, corner_radius);
        p->setBrush (Qt::NoBrush);

        // Paint button shadow
        p->setPen (!isEnabled ? QPen(dark_outline.lighter(115)) : QPen(dark_outline));
        p->drawRoundedRect (button_shape, corner_radius, corner_radius);

        // Paint semi-transparent inner outline
        p->setPen (QColor(255, 255, 255, 30));
        p->drawRoundedRect (button_shape.adjusted(1, 1, -1, -1), corner_radius, corner_radius);

        break;
    }
    case PE_Frame: {
        if (widget && widget->inherits("QComboBoxPrivateContainer")){
            QStyleOption copy = *option;
            copy.state |= State_Raised;
            proxy()->drawPrimitive(PE_PanelMenu, &copy, painter, widget);
            break;
        }
        painter->save();
        painter->setRenderHint (QPainter::Antialiasing, true);
        painter->translate (0.5, 0.5);

        painter->setPen (Qt::transparent);
        painter->setBrush (background_color.darker(155));
        painter->drawRoundedRect (option->rect, corner_radius/3, corner_radius/3);
        painter->setBrush (Qt::NoBrush);

        QPen pen(QColor(255,255,255,2));
        pen.setWidth(2);
        pen.setCosmetic(false);
        painter->setPen(pen);
        painter->drawRoundedRect(option->rect.adjusted(1,1,-1,-1), corner_radius/3, corner_radius/3);
        painter->restore();
        break;
    }
    default:
        QProxyStyle::drawPrimitive (element, option, painter, widget);
    }
}

