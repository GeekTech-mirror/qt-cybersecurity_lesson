#include <QDebug>
#include <QStyleOption>
#include <QStringBuilder>

#include "custom_stylesheets.h"
#include "custom_colors.h"

CustomStyleSheets::CustomStyleSheets()
    : sb_colors (new CustomColors::ScrollBar_Colors)
{
}


// Consider flickable for better touch compatibility (ScrollDecorator)
QString CustomStyleSheets::vertical_scrollbar ()
{
    QString stylesheet =
    "QScrollBar::Vertical { \
         border: none; \
         background: transparent; \
         padding-left: %2px; \
         padding-right: %2px; \
         border-radius: 0px; \
    }";
    stylesheet = stylesheet.arg(sb_metrics.padding);


    stylesheet +=
    "QScrollBar::handle:vertical { \
         border: 0.5px solid rgb(%1,%2,%3); \
         background-color: rgb(%4,%5,%6); \
         min-height: 30px; \
         border-radius: %7px; \
    }";
    stylesheet = stylesheet.arg(sb_colors->handle_outline.red())
                           .arg(sb_colors->handle_outline.green())
                           .arg(sb_colors->handle_outline.blue());

    stylesheet = stylesheet.arg(sb_colors->handle_bgColor.red())
                           .arg(sb_colors->handle_bgColor.green())
                           .arg(sb_colors->handle_bgColor.blue());

    stylesheet = stylesheet.arg(sb_metrics.handle_radius);


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


QString CustomStyleSheets::horizontal_scrollbar (void)
{
    QString stylesheet =
    "QScrollBar::Horizontal { \
         border: none; \
         background: transparent; \
         padding-top: %2px; \
         padding-bottom: %2px; \
         border-radius: 0px; \
    }";
    stylesheet = stylesheet.arg(sb_metrics.padding);


    stylesheet +=
    "QScrollBar::handle:horizontal { \
         border: 0.5px solid rgb(%1,%2,%3); \
         background-color: rgb(%4,%5,%6); \
         min-width: 30px; \
         border-radius: %7px; \
    }";
    stylesheet = stylesheet.arg(sb_colors->handle_outline.red())
                           .arg(sb_colors->handle_outline.green())
                           .arg(sb_colors->handle_outline.blue());

    stylesheet = stylesheet.arg(sb_colors->handle_bgColor.red())
                           .arg(sb_colors->handle_bgColor.green())
                           .arg(sb_colors->handle_bgColor.blue());

    stylesheet = stylesheet.arg(sb_metrics.handle_radius);


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


QString CustomStyleSheets::treeview_scrollbar (int header_height)
{
    setHeaderHeight(header_height);

    /* Vertical ScrollBar */
    QString stylesheet =
    "QScrollBar::Vertical { \
         border-left: 1px solid rgba(%1,%2,%3,%4); \
         border-right: 1px solid rgba(%5,%6,%7,%8); \
         background: rgb(%9,%10,%11); \
         padding-top: %12px; \
         padding-bottom: 10px; \
    }";

    stylesheet = stylesheet.arg(CustomColors::inner_contrast_line().red())
                           .arg(CustomColors::inner_contrast_line().green())
                           .arg(CustomColors::inner_contrast_line().blue())
                           .arg(CustomColors::inner_contrast_line().alpha());

    stylesheet = stylesheet.arg(CustomColors::outer_frame_border_color().red())
                           .arg(CustomColors::outer_frame_border_color().green())
                           .arg(CustomColors::outer_frame_border_color().blue())
                           .arg(CustomColors::outer_frame_border_color().alpha());

    stylesheet = stylesheet.arg(CustomColors::frame_color().red())
                           .arg(CustomColors::frame_color().green())
                           .arg(CustomColors::frame_color().blue());

    stylesheet = stylesheet.arg(sb_metrics.treeview_scrollbar_padding);


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
    // header top border
    stylesheet = stylesheet.arg(CustomColors::inner_contrast_line().red())
                           .arg(CustomColors::inner_contrast_line().green())
                           .arg(CustomColors::inner_contrast_line().blue())
                           .arg(CustomColors::inner_contrast_line().alpha());

    // header bottom border
    stylesheet = stylesheet.arg(CustomColors::outline().red())
                           .arg(CustomColors::outline().green())
                           .arg(CustomColors::outline().blue());

    // header gradient
    stylesheet = stylesheet.arg(CustomColors::header_gradientStart().red())
                           .arg(CustomColors::header_gradientStart().green())
                           .arg(CustomColors::header_gradientStart().blue());

    stylesheet = stylesheet.arg(CustomColors::header_midColor1().red())
                           .arg(CustomColors::header_midColor1().green())
                           .arg(CustomColors::header_midColor1().blue());

    stylesheet = stylesheet.arg(CustomColors::header_midColor2().red())
                           .arg(CustomColors::header_midColor2().green())
                           .arg(CustomColors::header_midColor2().blue());

    stylesheet = stylesheet.arg(CustomColors::header_gradientStop().red())
                           .arg(CustomColors::header_gradientStop().green())
                           .arg(CustomColors::header_gradientStop().blue());

    stylesheet = stylesheet.arg(CustomColors::header_gradientStop().darker(104).red())
                           .arg(CustomColors::header_gradientStop().darker(104).green())
                           .arg(CustomColors::header_gradientStop().darker(104).blue());

    stylesheet = stylesheet.arg(sb_metrics.header_height);


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


    stylesheet = stylesheet.arg(CustomColors::inner_contrast_line().red())
                           .arg(CustomColors::inner_contrast_line().green())
                           .arg(CustomColors::inner_contrast_line().blue())
                           .arg(CustomColors::inner_contrast_line().alpha());

    stylesheet = stylesheet.arg(CustomColors::outer_frame_border_color().red())
                           .arg(CustomColors::outer_frame_border_color().green())
                           .arg(CustomColors::outer_frame_border_color().blue())
                           .arg(CustomColors::outer_frame_border_color().alpha());

    stylesheet = stylesheet.arg(CustomColors::frame_color().red())
                           .arg(CustomColors::frame_color().green())
                           .arg(CustomColors::frame_color().blue());

    stylesheet = stylesheet.arg(12);


    return stylesheet;
}

QString CustomStyleSheets::treeview_vertical_scrollbar_quirk (void)
{
    QString stylesheet =
    "QScrollBar::add-line:vertical { \
         border-bottom: 1px solid rgba(%5,%6,%7,%8); \
    }";

    stylesheet = stylesheet.arg(CustomColors::outer_frame_border_color().red())
                           .arg(CustomColors::outer_frame_border_color().green())
                           .arg(CustomColors::outer_frame_border_color().blue())
                           .arg(CustomColors::outer_frame_border_color().alpha());

    return stylesheet;
}

void CustomStyleSheets::setHeaderHeight(int height)
{
    sb_metrics.header_height = height - 2;
    sb_metrics.treeview_scrollbar_padding = height + 6;
}
