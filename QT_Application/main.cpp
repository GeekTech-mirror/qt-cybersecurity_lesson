#include "main_window.h"
#include "custom_style.h"

#include <QApplication>

int main (int argc, char *argv[])
{
    QApplication::setStyle(new CustomStyle);

    QApplication app (argc, argv);

    Main_Window w;

    w.show();
    return app.exec ();
}
