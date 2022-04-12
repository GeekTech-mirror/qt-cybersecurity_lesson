#ifndef STYLE_CUSTOM_P_H
#define STYLE_CUSTOM_P_H

#include <QApplication>
#include <QCommonStyle>
#include <QStringBuilder>
#include <QStyleOptionComplex>


// internal helper. Converts an integer value to a unique string token
template <typename T>
        struct HexString
{
    inline HexString(const T t)
        : val(t)
    {}

    inline void write(QChar *&dest) const
    {
        const char16_t hexChars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
        const char *c = reinterpret_cast<const char *>(&val);
        for (uint i = 0; i < sizeof(T); ++i) {
            *dest++ = hexChars[*c & 0xf];
            *dest++ = hexChars[(*c & 0xf0) >> 4];
            ++c;
        }
    }
    const T val;
};


// specialization to enable fast concatenating of our string tokens to a string
template <typename T>
        struct QConcatenable<HexString<T> >
{
    typedef HexString<T> type;
    enum { ExactSize = true };
    static int size(const HexString<T> &) { return sizeof(T) * 2; }
    static inline void appendTo(const HexString<T> &str, QChar *&out) { str.write(out); }
    typedef QString ConvertTo;
};


class QStyleCustomPrivate
{
public:
    QStyleCustomPrivate();

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
            highlighted_outline.setHsl(highlighted_outline.hue(),
                                       highlighted_outline.saturation(),
                                       160);
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
        button_color.setHsv(button_color.hue(),
                            button_color.saturation() * 0.75,
                            button_color.value());
        return button_color;
    }

    enum {
        menuItemHMargin      =  3, // menu item hor text margin
        menuArrowHMargin     =  6, // menu arrow horizontal margin
        menuRightBorder      = 15, // right border on menus
        menuCheckMarkWidth   = 12  // checkmarks width on menus
    };

    QString unique_name(const QString &key, const QStyleOption *option, const QSize size)
    {
        const QStyleOptionComplex *complexOption = qstyleoption_cast<const QStyleOptionComplex *>(option);
        QString tmp = key % HexString<uint>(option->state)
                          % HexString<uint>(option->direction)
                          % HexString<uint>(complexOption ? uint(complexOption->activeSubControls) : 0u)
                          % HexString<quint64>(option->palette.cacheKey())
                          % HexString<uint>(size.width())
                          % HexString<uint>(size.height());

        return tmp;
    }

    inline QPixmap style_cache_pixmap(const QSize size)
    {
        const qreal pixelRatio = qApp->devicePixelRatio();
        QPixmap cachePixmap = QPixmap(size * pixelRatio);
        cachePixmap.setDevicePixelRatio(pixelRatio);
        return cachePixmap;
    }
};

#endif // STYLE_CUSTOM_P_H
