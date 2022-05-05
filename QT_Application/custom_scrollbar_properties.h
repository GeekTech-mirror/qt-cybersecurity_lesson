#ifndef CUSTOMSCROLLBAR_H
#define CUSTOMSCROLLBAR_H

#include <QtQml>

class CustomScrollBar_Properties
{
    Q_OBJECT
    QML_ELEMENT
public:
    CustomScrollBar_Properties();

    /* ScrollBar Colors */
        // Handle
        const QColor handle_bgColor =
                CustomColors::merged_colors (CustomColors::highlight(),
                                             CustomColors::background_color(),
                                             40);
        const QColor handle_outline =
                CustomColors::merged_colors (CustomColors::highlight(),
                                             CustomColors::background_color(),
                                             80);

    /* Unique ScrollBar Colors for TreeView Frame */
    struct ScrollBar_TreeView
    {
        /* ScrollBar colors */
        const QColor background = CustomColors::background_color().darker(145);
        const QColor inner_border = QColor (255, 255, 255, 30);
        const QColor outer_border =
                (CustomColors::merged_colors(CustomColors::highlight(),
                                             CustomColors::background_color().darker(145),
                                             90));;

        /* Header colors */
        const QColor gradient_start = CustomColors::background_color().lighter(130);
        const QColor gradient_stop = CustomColors::background_color().lighter(123);

        const QColor midColor1 = CustomColors::merged_colors (gradient_start, gradient_stop, 60);
        const QColor midColor2 = CustomColors::merged_colors (gradient_start, gradient_stop, 40);

        const QColor header_shadow = QColor (21,24,28);
    };

protected:
};

#endif // CUSTOMSCROLLBAR_H
