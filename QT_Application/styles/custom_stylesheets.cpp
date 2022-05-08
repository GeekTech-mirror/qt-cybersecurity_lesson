#include <QDebug>
#include <QStyleOption>
#include <QStringBuilder>

#include "custom_stylesheets.h"
#include "custom_colors.h"

CustomStyleSheets::CustomStyleSheets()
    : sb_colors (new CustomColors::ScrollBar_Colors),
      sb_treeview (new CustomColors::ScrollBar_TreeView)
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
    stylesheet = stylesheet.arg(sb_treeview->inner_border.red())
                           .arg(sb_treeview->inner_border.green())
                           .arg(sb_treeview->inner_border.blue())
                           .arg(sb_treeview->inner_border.alpha());

    stylesheet = stylesheet.arg(sb_treeview->outer_border.red())
                           .arg(sb_treeview->outer_border.green())
                           .arg(sb_treeview->outer_border.blue())
                           .arg(sb_treeview->outer_border.alpha());

    stylesheet = stylesheet.arg(sb_treeview->background.red())
                           .arg(sb_treeview->background.green())
                           .arg(sb_treeview->background.blue());

    stylesheet = stylesheet.arg(sb_metrics.header_padding);


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
     stylesheet = stylesheet.arg(sb_treeview->inner_border.red())
                            .arg(sb_treeview->inner_border.green())
                            .arg(sb_treeview->inner_border.blue())
                            .arg(sb_treeview->inner_border.alpha());

    stylesheet = stylesheet.arg(sb_treeview->header_shadow.red())
                           .arg(sb_treeview->header_shadow.green())
                           .arg(sb_treeview->header_shadow.blue());

    // header gradient
    stylesheet = stylesheet.arg(sb_treeview->gradient_start.red())
                           .arg(sb_treeview->gradient_start.green())
                           .arg(sb_treeview->gradient_start.blue());

    stylesheet = stylesheet.arg(sb_treeview->midColor1.red())
                           .arg(sb_treeview->midColor1.green())
                           .arg(sb_treeview->midColor1.blue());

    stylesheet = stylesheet.arg(sb_treeview->midColor2.red())
                           .arg(sb_treeview->midColor2.green())
                           .arg(sb_treeview->midColor2.blue());

    stylesheet = stylesheet.arg(sb_treeview->gradient_stop.red())
                           .arg(sb_treeview->gradient_stop.green())
                           .arg(sb_treeview->gradient_stop.blue());

    stylesheet = stylesheet.arg(sb_treeview->gradient_stop.darker(104).red())
                           .arg(sb_treeview->gradient_stop.darker(104).green())
                           .arg(sb_treeview->gradient_stop.darker(104).blue());

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
     stylesheet = stylesheet.arg(sb_treeview->inner_border.red())
                            .arg(sb_treeview->inner_border.green())
                            .arg(sb_treeview->inner_border.blue())
                            .arg(sb_treeview->inner_border.alpha());

     stylesheet = stylesheet.arg(sb_treeview->outer_border.red())
                            .arg(sb_treeview->outer_border.green())
                            .arg(sb_treeview->outer_border.blue())
                            .arg(sb_treeview->outer_border.alpha());

     stylesheet = stylesheet.arg(sb_treeview->background.red())
                            .arg(sb_treeview->background.green())
                            .arg(sb_treeview->background.blue());

     stylesheet = stylesheet.arg(12);


    return stylesheet;
}

QString CustomStyleSheets::treeview_vertical_scrollbar_quirk (void)
{
    QString stylesheet =
    "QScrollBar::add-line:vertical { \
         border-bottom: 1px solid rgba(%5,%6,%7,%8); \
    }";

    stylesheet = stylesheet.arg(sb_treeview->outer_border.red())
                           .arg(sb_treeview->outer_border.green())
                           .arg(sb_treeview->outer_border.blue())
                           .arg(sb_treeview->outer_border.alpha());

    return stylesheet;
}

void CustomStyleSheets::setHeaderHeight(int height)
{
    sb_metrics.header_height = height - 2;
    sb_metrics.header_padding = height + 6;
}
