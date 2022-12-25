#ifndef RUBBERDUCKY_H
#define RUBBERDUCKY_H

#include <Solid/StorageAccess>

#include <QStringListModel>
#include <QWidget>

/* local include files */
#include "custom_stylesheets.h"

namespace Ui {
class RubberDucky;
}

class RubberDucky : public QWidget
{
    Q_OBJECT

public:
    explicit RubberDucky(QWidget *parent = nullptr);
    ~RubberDucky();

private slots:
    void insert_command(const QString &cmd = "");
    void insert_newline();
    void remove_current_line();
    void clear_current_line();

    void cmd_copy_image_clicked();
    void cmd_mount_usb_clicked();

    void cmd_open_cmdline_clicked();
    void cmd_open_image_clicked();

    void cmd_s_z_clicked();

    void save_to_file();
    void prepare_ducky();

    QByteArray encode_script (QStringList &duck_script, QJsonObject &lang);
    void duckytools(QJsonObject &lang);

    void copy_to_pico (QString srcpath, QObject *sender, Solid::ErrorType error);

private:
    Ui::RubberDucky *ui;

    QStringListModel *script_model;
    QStringList *script_list;

    CustomStyleSheets *stylesheets;
    QString listview_stylesheet;
    QString rubberducky_stylesheet();

    void setup_cmd_buttons();

    void copy_folder (QString srcpath, QString destpath);

protected:
    bool eventFilter (QObject *object, QEvent *event) override;
};

#endif // RUBBERDUCKY_H
