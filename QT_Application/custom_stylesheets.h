#ifndef CUSTOMSTYLESHEETS_H
#define CUSTOMSTYLESHEETS_H

#include <QMainWindow>

#include "custom_colors.h"
#include "custom_metrics.h"

class CustomStyleSheets
{
public:
    CustomStyleSheets ();

    QString scrollbar_vertical (void);
    QString scrollbar_horizontal (void);
    QString scrollbar_treeview (int);



private:
    void setHeaderHeight(int);

    ScrollBar_Metrics sb_metrics;
    const CustomColors::ScrollBar_Colors *sb_colors;
    const CustomColors::ScrollBar_TreeView *sb_treeview;

    //int header_height = 27;
};

#endif // CUSTOMSTYLESHEETS_H
