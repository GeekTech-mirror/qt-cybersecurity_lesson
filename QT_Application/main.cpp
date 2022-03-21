#include "main_window.h"
#include "style_custom.h"

#include <QApplication>

#include <arpa/inet.h>
#include <QThread>
#include <QTextStream>
#include <QStandardItem>
#include <QDebug>

#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Device>
#include <NetworkManagerQt/WirelessDevice>
#include <NetworkManagerQt/AccessPoint>

int main (int argc, char *argv[])
{
    QApplication::setStyle(new QStyleCustom);

    QApplication app (argc, argv);

    Main_Window w;

    w.show();
    return app.exec ();
}
