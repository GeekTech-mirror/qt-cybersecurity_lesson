#include "main_window.h"
#include "button_style.h"

#include <QApplication>

int main (int argc, char *argv[])
{
//    Q_INIT_RESOURCE(styles);

//    QApplication::setStyle(new button_style);

    QApplication app (argc, argv);

    Main_Window w;
    w.show();
    return app.exec ();
}
