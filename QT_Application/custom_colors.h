#ifndef CUSTOMCOLORS_H
#define CUSTOMCOLORS_H

#include <QColor>
#include <QCommonStyle>
#include <QLinearGradient>



class CustomColors
{
private:
    enum Direction {
        TopDown,
        FromLeft,
        BottomUp,
        FromRight
    };

public:
    CustomColors();

    static QColor merged_colors (const QColor &colorA,
                                 const QColor &colorB,
                                 int factor = 50);

    static QLinearGradient qt_fusion_gradient (const QRect rect,
                                               const QBrush &baseColor,
                                               Direction direction = TopDown);

    /* Color Palette */
    static QColor light_shade() {
        return QColor(255, 255, 255, 90);
    }
    static QColor dark_shade() {
        return QColor(0, 0, 0, 60);
    }

    static QColor top_shadow() {
        return QColor(0, 0, 0, 18);
    }

    static QColor inner_contrast_line() {
        return QColor(255, 255, 255, 30);
    }

    static QColor highlight(const QPalette &pal) {
        return pal.color(QPalette::Highlight);
    }

    static QColor highlight(void) {
        return QColor(61,174,233);
    }

    static QColor highlighted_text(const QPalette &pal) {
        return pal.color(QPalette::HighlightedText);
    }

    static QColor outline(const QPalette &pal) {
        if (pal.window().style() == Qt::TexturePattern)
            return QColor(0, 0, 0, 160);
        return pal.window().color().darker(140);
    }

    static QColor highlighted_outline(const QPalette &pal) {
        QColor highlighted_outline = highlight(pal).darker(125);
        if (highlighted_outline.value() > 160)
            highlighted_outline.setHsl(highlighted_outline.hue(),
                                       highlighted_outline.saturation(),
                                       160);
        return highlighted_outline;
    }

    static QColor tab_frame_color(const QPalette &pal) {
        if (pal.window().style() == Qt::TexturePattern)
            return QColor(255, 255, 255, 8);
        return background_color().lighter(104);
    }

    static QColor background_color() {
        return QColor(40,44,52);
    }



};

#endif // CUSTOMCOLORS_H
