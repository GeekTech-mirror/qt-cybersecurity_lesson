#ifndef CUSTOMSTYLESHEETS_H
#define CUSTOMSTYLESHEETS_H

#include <QMainWindow>

#include "custom_colors.h"

class CustomStyleSheets
{
public:
    CustomStyleSheets ();

    QString scrollbar_vertical (void);
    QString scrollbar_horizontal (void);
    QString scrollbar_treeview (void);

    void setHeaderHeight(int);

private:
    struct ScrollBar_Metrics
    {
        // ScrollBar
        const int width = 22;
        const int padding = 4; // distance between handle and border (floating handle)

        // Handle
        const int handle_radius = 6;

        // TreeView Settings
        int header_padding = 35;
        int header_height = 27;
    };

    ScrollBar_Metrics *sb_metrics;

    const CustomColors::ScrollBar_Colors *sb_colors;
    const CustomColors::ScrollBar_TreeView *sb_treeview;

    int header_height = 27;


};

#endif // CUSTOMSTYLESHEETS_H
