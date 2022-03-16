#ifndef STYLE_COMMON_H
#define STYLE_COMMON_H

#include <QCommonStyle>
#include <QGuiApplication>

class QStyleCommon
{
public:
    QStyleCommon()
    {
    };

    // Used for grip handles
    QColor light_shade() const {
        return QColor(255, 255, 255, 90);
    }
    QColor dark_shade() const {
        return QColor(0, 0, 0, 60);
    }

    QColor top_shadow() const {
        return QColor(0, 0, 0, 18);
    }

    QColor inner_contrast_line() const {
        return QColor(255, 255, 255, 30);
    }

    QColor highlight(const QPalette &pal) const {
        return pal.color(QPalette::Highlight);
    }

    QColor highlighted_text(const QPalette &pal) const {
        return pal.color(QPalette::HighlightedText);
    }

    QColor outline(const QPalette &pal) const {
        if (pal.window().style() == Qt::TexturePattern)
            return QColor(0, 0, 0, 160);
        return pal.window().color().darker(140);
    }

    QColor highlighted_outline(const QPalette &pal) const {
        QColor highlighted_outline = highlight(pal).darker(125);
        if (highlighted_outline.value() > 160)
            highlighted_outline.setHsl(highlighted_outline.hue(), highlighted_outline.saturation(), 160);
        return highlighted_outline;
    }

    QColor tab_frame_color(const QPalette &pal) const {
        if (pal.window().style() == Qt::TexturePattern)
            return QColor(255, 255, 255, 8);
        return button_color(pal).lighter(104);
    }

    QColor button_color(const QPalette &pal) const {
        QColor button_color = pal.button().color();
        int val = qGray(button_color.rgb());
        button_color = button_color.lighter(100 + qMax(1, (180 - val)/6));
        button_color.setHsv(button_color.hue(), button_color.saturation() * 0.75, button_color.value());
        return button_color;
    }

    enum {
        menuItemHMargin      =  3, // menu item hor text margin
        menuArrowHMargin     =  6, // menu arrow horizontal margin
        menuRightBorder      = 15, // right border on menus
        menuCheckMarkWidth   = 12  // checkmarks width on menus
    };
};

#endif // STYLE_COMMON_H
