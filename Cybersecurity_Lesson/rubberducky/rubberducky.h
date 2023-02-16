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

    void duckytools(QJsonObject &lang);

    void copy_to_pico (QString srcpath, QObject *sender, Solid::ErrorType error);

    QByteArray encode_script (QStringList &duck_script, QJsonObject &lang);
    void parse_instruction (const QString &cmd, const QString &instruction,
                            QByteArray *encoded_script, const QJsonObject& lang);
    QByteArray delay_bytes(uint delay);

private:
    Ui::RubberDucky *ui;

    QStringListModel *script_model;
    QStringList *script_list;

    CustomStyleSheets *stylesheets;
    //QString listview_stylesheet;
    QString rubberducky_stylesheet();

    void setup_search_paths();

    void setup_cmd_buttons();

    void setup_rubberducky_theme();

    void mount_pico(QString &srcpath);
    void copy_folder (QString &srcpath, QString &destpath);

protected:
    //bool eventFilter (QObject *object, QEvent *event) override;
};

#endif // RUBBERDUCKY_H
