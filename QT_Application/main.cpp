#include "main_window.h"
#include "style_custom.h"

#include <QApplication>

int main (int argc, char *argv[])
{
    QApplication::setStyle(new QStyleCustom);

    QApplication app (argc, argv);

    Main_Window w;

    w.show();
    return app.exec ();
}
