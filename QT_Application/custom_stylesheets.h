#ifndef CUSTOMSTYLESHEETS_H
#define CUSTOMSTYLESHEETS_H

#include <QMainWindow>

#include "main_window.h"

class CustomStyleSheets
{
public:
    CustomStyleSheets ();

    static QString scrollbar_vertical (void);
    static QString scrollbar_horizontal (void);
    static QString scrollbar_treeview (void);
};

#endif // CUSTOMSTYLESHEETS_H
