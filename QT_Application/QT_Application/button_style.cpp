#include "button_style.h"

#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include <QComboBox>
#include <QStyleFactory>
#include <QStyleOption>


button_style::button_style() :
    QProxyStyle(QStyleFactory::create("fusion"))
{
    setObjectName("button_style");
}


QPalette button_style::standardPalette() const
{
    QColor backGround(239, 239, 239);
    QColor light = backGround.lighter(150);
    QColor mid(backGround.darker(130));
    QColor midLight = mid.lighter(110);
    QColor base = Qt::white;
    QColor disabledBase(backGround);
    QColor dark = backGround.darker(150);
    QColor darkDisabled = QColor(209, 209, 209).darker(110);
    QColor text = Qt::black;
    QColor hightlightedText = Qt::white;
    QColor disabledText = QColor(190, 190, 190);
    QColor button = backGround;
    QColor shadow = dark.darker(135);
    QColor disabledShadow = shadow.lighter(150);
    QColor placeholder = text;
    placeholder.setAlpha(128);

    QPalette fusionPalette(Qt::black,backGround,light,dark,mid,text,base);
    fusionPalette.setBrush(QPalette::Midlight, midLight);
    fusionPalette.setBrush(QPalette::Button, button);
    fusionPalette.setBrush(QPalette::Shadow, shadow);
    fusionPalette.setBrush(QPalette::HighlightedText, hightlightedText);

    fusionPalette.setBrush(QPalette::Disabled, QPalette::Text, disabledText);
    fusionPalette.setBrush(QPalette::Disabled, QPalette::WindowText, disabledText);
    fusionPalette.setBrush(QPalette::Disabled, QPalette::ButtonText, disabledText);
    fusionPalette.setBrush(QPalette::Disabled, QPalette::Base, disabledBase);
    fusionPalette.setBrush(QPalette::Disabled, QPalette::Dark, darkDisabled);
    fusionPalette.setBrush(QPalette::Disabled, QPalette::Shadow, disabledShadow);

    fusionPalette.setBrush(QPalette::Active, QPalette::Highlight, QColor(48, 140, 198));
    fusionPalette.setBrush(QPalette::Inactive, QPalette::Highlight, QColor(48, 140, 198));
    fusionPalette.setBrush(QPalette::Disabled, QPalette::Highlight, QColor(145, 145, 145));

    fusionPalette.setBrush(QPalette::PlaceholderText, placeholder);

    return fusionPalette;
}


void button_style::drawPrimitive(PrimitiveElement element,
                                       const QStyleOption *option,
                                       QPainter *painter,
                                       const QWidget *widget) const
{
    /*
    Q_ASSERT(option);
    Q_D (const QFusionStyle);

    QRect rect = option->rect;
    int state = option->state;

    QColor outline = d->outline(option->palette);
    QColor highlightedOutline = d->highlightedOutline(option->palette);

    QColor tabFrameColor = d->tabFrameColor(option->palette);
*/
    switch (element) {
    case PE_PanelButtonCommand:
        {
            int delta = (option->state & State_MouseOver) ? 64 : 0;
            QColor slightlyOpaqueBlack(0, 0, 0, 63);
            QColor semiTransparentWhite(255, 255, 255, 127 + delta);
            QColor semiTransparentBlack(0, 0, 0, 127 - delta);

            int x, y, width, height;
            option->rect.getRect(&x, &y, &width, &height);
            QPainterPath roundRect = roundRectPath(option->rect);

            int radius = qMin(width, height) / 12;

            QBrush brush;
            bool darker;

            const QStyleOptionButton *buttonOption =
                    qstyleoption_cast<const QStyleOptionButton *>(option);
            if (buttonOption
                    && (buttonOption->features & QStyleOptionButton::Flat)) {
                brush = option->palette.window();
                darker = (option->state & (State_Sunken | State_On));
            } else {
                if (option->state & (State_Sunken | State_On)) {
                    brush = option->palette.mid();
                    darker = !(option->state & State_Sunken);
                } else {
                    brush = option->palette.button();
                    darker = false;
                }
            }

            painter->save();
            painter->setRenderHint(QPainter::Antialiasing, true);
            painter->fillPath(roundRect, brush);

            if (darker)
                painter->fillPath(roundRect, slightlyOpaqueBlack);

            int penWidth;
            if (radius < 10)
                penWidth = 3;
            else if (radius < 20)
                penWidth = 5;
            else
                penWidth = 7;

            QPen topPen(semiTransparentWhite, penWidth);
            QPen bottomPen(semiTransparentBlack, penWidth);

            if (option->state & (State_Sunken | State_On))
                qSwap(topPen, bottomPen);

            int x1 = x;
            int x2 = x + radius;
            int x3 = x + width - radius;
            int x4 = x + width;

            if (option->direction == Qt::RightToLeft) {
                qSwap(x1, x4);
                qSwap(x2, x3);
            }

            QPolygon topHalf;
            topHalf << QPoint(x1, y)
                    << QPoint(x4, y)
                    << QPoint(x3, y + radius)
                    << QPoint(x2, y + height - radius)
                    << QPoint(x1, y + height);

            painter->setClipPath(roundRect);
            painter->setClipRegion(topHalf, Qt::IntersectClip);
            painter->setPen(topPen);
            painter->drawPath(roundRect);

            QPolygon bottomHalf = topHalf;
            bottomHalf[0] = QPoint(x4, y + height);

            painter->setClipPath(roundRect);
            painter->setClipRegion(bottomHalf, Qt::IntersectClip);
            painter->setPen(bottomPen);
            painter->drawPath(roundRect);

            painter->setPen(option->palette.windowText().color());
            painter->setClipping(false);
            painter->drawPath(roundRect);

            painter->restore();
        }
        break;
    default:
        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
}


void button_style::drawControl(ControlElement element,
                                     const QStyleOption *option,
                                     QPainter *painter,
                                     const QWidget *widget) const
{
    switch (element) {
    case CE_PushButtonLabel:
        {
            QStyleOptionButton myButtonOption;
            const QStyleOptionButton *buttonOption =
                    qstyleoption_cast<const QStyleOptionButton *>(option);
            if (buttonOption) {
                myButtonOption = *buttonOption;
                if (myButtonOption.palette.currentColorGroup()
                        != QPalette::Disabled) {
                    if (myButtonOption.state & (State_Sunken | State_On)) {
                        myButtonOption.palette.setBrush(QPalette::ButtonText,
                                myButtonOption.palette.brightText());
                    }
                }
            }
            QProxyStyle::drawControl(element, &myButtonOption, painter, widget);
        }
        break;
    default:
        QProxyStyle::drawControl(element, option, painter, widget);
    }
}


void button_style::polish(QWidget *widget)
{
    if (qobject_cast<QPushButton *>(widget)
            || qobject_cast<QComboBox *>(widget))
        widget->setAttribute(Qt::WA_Hover, true);
}


void button_style::unpolish(QWidget *widget)
{
    if (qobject_cast<QPushButton *>(widget)
            || qobject_cast<QComboBox *>(widget))
        widget->setAttribute(Qt::WA_Hover, false);
}


int button_style::pixelMetric(PixelMetric metric,
                                    const QStyleOption *option,
                                    const QWidget *widget) const
{
    switch (metric) {
    case PM_ComboBoxFrameWidth:
        return 8;
    case PM_ScrollBarExtent:
        return QProxyStyle::pixelMetric(metric, option, widget) + 4;
    default:
        return QProxyStyle::pixelMetric(metric, option, widget);
    }
}


int button_style::styleHint(StyleHint hint, const QStyleOption *option,
                                  const QWidget *widget,
                                  QStyleHintReturn *returnData) const
{
    switch (hint) {
    case SH_DitherDisabledText:
        return int(false);
    case SH_EtchDisabledText:
        return int(true);
    default:
        return QProxyStyle::styleHint(hint, option, widget, returnData);
    }
}


QPainterPath button_style::roundRectPath(const QRect &rect)
{
    int radius = qMin(rect.width(), rect.height()) / 2;
    int diam = 2 * radius;

    int x1, y1, x2, y2;
    rect.getCoords(&x1, &y1, &x2, &y2);

    QPainterPath path;
    path.moveTo(x2, y1 + radius);
    path.arcTo(QRect(x2 - diam, y1, diam, diam), 0.0, +90.0);
    path.lineTo(x1 + radius, y1);
    path.arcTo(QRect(x1, y1, diam, diam), 90.0, +90.0);
    path.lineTo(x1, y2 - radius);
    path.arcTo(QRect(x1, y2 - diam, diam, diam), 180.0, +90.0);
    path.lineTo(x1 + radius, y2);
    path.arcTo(QRect(x2 - diam, y2 - diam, diam, diam), 270.0, +90.0);
    path.closeSubpath();
    return path;
}
