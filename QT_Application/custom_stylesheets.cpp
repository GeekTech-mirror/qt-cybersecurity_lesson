#include <QDebug>
#include <QStyleOption>
#include <QStringBuilder>

#include "custom_stylesheets.h"
#include "custom_colors.h"

CustomStyleSheets::CustomStyleSheets()
{
}

struct ScrollBar_Colors
{
    // ScrollBar
    const int width = 22;
    const int padding = 4; // distance between handle and border

    // Handle
    const QColor handle_bgColor =
            CustomColors::merged_colors (CustomColors::highlight(),
                                         CustomColors::background_color(),
                                         40);
    const QColor handle_outline =
            CustomColors::merged_colors (CustomColors::highlight(),
                                         CustomColors::background_color(),
                                         80);
    const int handle_radius = 6;
};

struct TreeView_ScrollBar
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
    const int header_height = 27;
};


QString CustomStyleSheets::scrollbar_vertical (void)
{
    ScrollBar_Colors sb_colors;
    QColor handle_bg = sb_colors.handle_bgColor;
    QColor handle_outline = sb_colors.handle_outline;

    QString stylesheet =
    "QScrollBar::Vertical { \
         border: none; \
         background: transparent; \
         width: %1px; \
         padding-left: %2px; \
         padding-right: %2px; \
         border-radius: 0px; \
    }";
    stylesheet = stylesheet.arg(sb_colors.width).arg(sb_colors.padding);


    stylesheet +=
    "QScrollBar::handle:vertical { \
         border: 0.5px solid rgb(%1,%2,%3); \
         background-color: rgb(%4,%5,%6); \
         min-height: 30px; \
         border-radius: %7px; \
    }";
    stylesheet = stylesheet.arg(handle_outline.red())
                           .arg(handle_outline.green())
                           .arg(handle_outline.blue());

    stylesheet = stylesheet.arg(handle_bg.red())
                           .arg(handle_bg.green())
                           .arg(handle_bg.blue());

    stylesheet = stylesheet.arg(sb_colors.handle_radius);


    stylesheet +=
    "QScrollBar::sub-line:vertical { \
         border: none; \
         background-color: transparent; \
         height: 0px; \
         subcontrol-position: top; \
         subcontrol-origin: margin; \
    }";


    stylesheet +=
    "QScrollBar::add-line:vertical { \
         border: none; \
         background-color: transparent; \
         height: 0px; \
         subcontrol-position: bottom; \
         subcontrol-origin: margin; \
    }";

    return stylesheet;
}


QString CustomStyleSheets::scrollbar_horizontal (void)
{
    ScrollBar_Colors sb_colors;
    QColor handle_bg = sb_colors.handle_bgColor;
    QColor handle_outline = sb_colors.handle_outline;

    QString stylesheet =
    "QScrollBar::Horizontal { \
         border: none; \
         background: transparent; \
         height: %1px; \
         padding-top: %2px; \
         padding-bottom: %2px; \
         border-radius: 0px; \
    }";
    stylesheet = stylesheet.arg(sb_colors.width).arg(sb_colors.padding);


    stylesheet +=
    "QScrollBar::handle:horizontal { \
         border: 0.5px solid rgb(%1,%2,%3); \
         background-color: rgb(%4,%5,%6); \
         min-width: 30px; \
         border-radius: %7px; \
    }";
    stylesheet = stylesheet.arg(handle_outline.red())
                           .arg(handle_outline.green())
                           .arg(handle_outline.blue());

    stylesheet = stylesheet.arg(handle_bg.red())
                           .arg(handle_bg.green())
                           .arg(handle_bg.blue());

    stylesheet = stylesheet.arg(sb_colors.handle_radius);


    stylesheet +=
    "QScrollBar::sub-line:horizontal { \
         border: none; \
         background-color: transparent; \
         width: 0px; \
         subcontrol-position: left; \
         subcontrol-origin: margin; \
    }";


    stylesheet +=
    "QScrollBar::add-line:horizontal { \
         border: none; \
         background-color: transparent; \
         width: 0px; \
         subcontrol-position: right; \
         subcontrol-origin: margin; \
    }";

    return stylesheet;
}


QString CustomStyleSheets::scrollbar_treeview (void)
{
    TreeView_ScrollBar sb_treeview;
    QColor background = sb_treeview.background;
    QColor inner_border = sb_treeview.inner_border;
    QColor outer_border = sb_treeview.outer_border;

    QColor gradient_start = sb_treeview.gradient_start;
    QColor gradient_stop = sb_treeview.gradient_stop;
    QColor midColor1 = sb_treeview.midColor1;
    QColor midColor2 = sb_treeview.midColor2;

    QColor header_shadow = sb_treeview.header_shadow;

    /* Vertical ScrollBar */
    QString stylesheet =
    "QScrollBar::Vertical { \
         border-left: 1px solid rgba(%1,%2,%3,%4); \
         border-right: 1px solid rgba(%5,%6,%7,%8); \
         background: rgb(%9,%10,%11); \
         padding-top: 35px; \
         padding-bottom: 10px; \
    }";
    stylesheet = stylesheet.arg(inner_border.red())
                           .arg(inner_border.green())
                           .arg(inner_border.blue())
                           .arg(inner_border.alpha());

    stylesheet = stylesheet.arg(outer_border.red())
                           .arg(outer_border.green())
                           .arg(outer_border.blue())
                           .arg(outer_border.alpha());

    stylesheet = stylesheet.arg(background.red())
                           .arg(background.green())
                           .arg(background.blue());


    stylesheet +=
    "QScrollBar::sub-line:vertical { \
         border-top: 1px solid rgba(%1,%2,%3,%4); \
         border-bottom: 1px solid rgb(%5,%6,%7); \
         background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, \
                                           stop:0 rgb(%8,%9,%10) \
                                           stop:0.5 rgb(%11,%12,%13) \
                                           stop:0.501 rgb(%14,%15,%16) \
                                           stop:0.92 rgb(%17,%18,%19) \
                                           stop:1 rgb(%20,%21,%22)); \
         height: %23px; \
    }";
     // header borders
     stylesheet = stylesheet.arg(inner_border.red())
                            .arg(inner_border.green())
                            .arg(inner_border.blue())
                            .arg(inner_border.alpha());

    stylesheet = stylesheet.arg(header_shadow.red())
                           .arg(header_shadow.green())
                           .arg(header_shadow.blue());

    // header gradient
    stylesheet = stylesheet.arg(gradient_start.red())
                           .arg(gradient_start.green())
                           .arg(gradient_start.blue());

    stylesheet = stylesheet.arg(midColor1.red())
                           .arg(midColor1.green())
                           .arg(midColor1.blue());

    stylesheet = stylesheet.arg(midColor2.red())
                           .arg(midColor2.green())
                           .arg(midColor2.blue());

    stylesheet = stylesheet.arg(gradient_stop.red())
                           .arg(gradient_stop.green())
                           .arg(gradient_stop.blue());

    stylesheet = stylesheet.arg(gradient_stop.darker(104).red())
                           .arg(gradient_stop.darker(104).green())
                           .arg(gradient_stop.darker(104).blue());

    stylesheet = stylesheet.arg(sb_treeview.header_height);


    /* Horizontal ScrollBar */
    stylesheet +=
    "QScrollBar::Horizontal { \
         border-top: 1px solid rgba(%1,%2,%3,%4); \
         border-bottom: 1px solid rgba(%5,%6,%7,%8); \
         border-left: 1px solid rgba(%5,%6,%7,%8); \
         background: rgb(%9,%10,%11); \
         padding-left: %12px; \
         padding-right: %12px; \
    }";
     stylesheet = stylesheet.arg(inner_border.red())
                            .arg(inner_border.green())
                            .arg(inner_border.blue())
                            .arg(inner_border.alpha());

     stylesheet = stylesheet.arg(outer_border.red())
                            .arg(outer_border.green())
                            .arg(outer_border.blue())
                            .arg(outer_border.alpha());

     stylesheet = stylesheet.arg(background.red())
                            .arg(background.green())
                            .arg(background.blue());

     stylesheet = stylesheet.arg(12);


    return stylesheet;
}
