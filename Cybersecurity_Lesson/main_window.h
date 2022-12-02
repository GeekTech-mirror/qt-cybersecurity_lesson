#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

/* Qt include files */
#include <QAbstractItemModel>
#include <QMainWindow>
#include <QStackedWidget>
#include <QFontDatabase>
#include <QFont>
#include <QIcon>
#include <QList>

/* local include files */
#include "custom_stylesheets.h"


QT_BEGIN_NAMESPACE
namespace Ui
{
    class Main_Window;
}
QT_END_NAMESPACE

class Main_Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Main_Window (QWidget *parent = nullptr);
    ~Main_Window ();

private slots:
    void on_rubber_ducky_button_clicked ();

    void on_wifi_deauther_button_clicked();

    void on_war_driving_button_clicked();

    void on_action_home_triggered();

    void on_action_close_triggered();


private:
    Ui::Main_Window *ui;

    QFont title_font = QFont("DejavuSerif", 40, QFont::Bold);
    QFont button_font = QFont("DejavuSans", 26, QFont::Bold);

    CustomStyleSheets *stylesheets;
    QString main_window_stylesheet();

    void setup_fonts (void);
    void setup_main_menu (void);
};


#endif // MAIN_WINDOW_H
