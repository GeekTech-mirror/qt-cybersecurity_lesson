#ifndef CUSTOMSTYLESHEETS_H
#define CUSTOMSTYLESHEETS_H

#include <QMainWindow>

#include "custom_colors.h"
#include "custom_metrics.h"

class CustomStyleSheets
{
public:
    CustomStyleSheets ();

    QString vertical_scrollbar (void);
    QString horizontal_scrollbar (void);
    QString treeview_scrollbar (int);
    QString treeview_vertical_scrollbar_quirk (void);


private:
    void setHeaderHeight (int);

    ScrollBar_Metrics sb_metrics;
    const CustomColors::ScrollBar_Colors *sb_colors;

    //int header_height = 27;
};

#endif // CUSTOMSTYLESHEETS_H
