#ifndef CUSTOM_BUTTON_H
#define CUSTOM_BUTTON_H

#include <QObject>
#include <QStyleFactory>
#include <QStyle>
#include <QPushButton>
#include <QPaintEvent>
#include <QPainter>
#include <QProxyStyle>
#include <QStyleOption>
#include <QPainter>

class Custom_Button : public QStyle
{
    Q_OBJECT
public:
    explicit Custom_Button(QStyle *parent = nullptr);

//    ~Custom_Button ();

    QPalette standardPalette () const override;
    void drawPrimitive(PrimitiveElement elem,
                       const QStyleOption *option,
                       QPainter *painter, const QWidget *widget = nullptr) const override;
    void drawControl(ControlElement ce, const QStyleOption *option, QPainter *painter,
                     const QWidget *widget) const override;
    int pixelMetric(PixelMetric metric, const QStyleOption *option = nullptr, const QWidget *widget = nullptr) const override;
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option,
                            QPainter *painter, const QWidget *widget) const override;
    QRect subElementRect(SubElement r, const QStyleOption *opt, const QWidget *widget = nullptr) const override;
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                           const QSize &size, const QWidget *widget) const override;
    SubControl hitTestComplexControl(ComplexControl cc, const QStyleOptionComplex *opt,
                                     const QPoint &pt, const QWidget *w = nullptr) const override;
    QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt,
                         SubControl sc, const QWidget *widget) const override;
    QPixmap generatedIconPixmap(QIcon::Mode iconMode, const QPixmap &pixmap,
                                const QStyleOption *opt) const override;
    int styleHint(StyleHint hint, const QStyleOption *option = nullptr, const QWidget *widget = nullptr,
                  QStyleHintReturn *returnData = nullptr) const override;
    QRect itemPixmapRect(const QRect &r, int flags, const QPixmap &pixmap) const override;
    QIcon standardIcon(StandardPixmap standardIcon, const QStyleOption *option = nullptr,
                       const QWidget *widget = nullptr) const override;
    QPixmap standardPixmap(StandardPixmap standardPixmap, const QStyleOption *opt,
                           const QWidget *widget = nullptr) const override;
    void drawItemPixmap(QPainter *painter, const QRect &rect,
                        int alignment, const QPixmap &pixmap) const override;
    void drawItemText(QPainter *painter, const QRect &rect,
                      int flags, const QPalette &pal, bool enabled,
                      const QString &text, QPalette::ColorRole textRole = QPalette::NoRole) const override;
    void polish(QWidget *widget) override;

    // Used for grip handles
    QColor lightShade() const {
        return QColor(255, 255, 255, 90);
    }
    QColor darkShade() const {
        return QColor(0, 0, 0, 60);
    }

    QColor topShadow() const {
        return QColor(0, 0, 0, 18);
    }

    QColor innerContrastLine() const {
        return QColor(255, 255, 255, 30);
    }

    QColor highlight(const QPalette &pal) const {
        return pal.color(QPalette::Highlight);
    }

    QColor highlightedText(const QPalette &pal) const {
        return pal.color(QPalette::HighlightedText);
    }

    QColor outline(const QPalette &pal) const {
        if (pal.window().style() == Qt::TexturePattern)
            return QColor(0, 0, 0, 160);
        return pal.window().color().darker(140);
    }

    QColor highlightedOutline(const QPalette &pal) const {
        QColor highlightedOutline = highlight(pal).darker(125);
        if (highlightedOutline.value() > 160)
            highlightedOutline.setHsl(highlightedOutline.hue(), highlightedOutline.saturation(), 160);
        return highlightedOutline;
    }

    QColor tabFrameColor(const QPalette &pal) const {
        if (pal.window().style() == Qt::TexturePattern)
            return QColor(255, 255, 255, 8);
        return buttonColor(pal).lighter(104);
    }

    QColor buttonColor(const QPalette &pal) const {
        QColor buttonColor = pal.button().color();
        int val = qGray(buttonColor.rgb());
        buttonColor = buttonColor.lighter(100 + qMax(1, (180 - val)/6));
        buttonColor.setHsv(buttonColor.hue(), buttonColor.saturation() * 0.75, buttonColor.value());
        return buttonColor;
    }

protected:
    void paintEvent (QPaintEvent *);

signals:

};

#endif // CUSTOM_BUTTON_H
