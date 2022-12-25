#include <QStringBuilder>

#include "languages.h"

#include "rubberducky.h"
#include "ui_rubberducky.h"


void RubberDucky::setup_cmd_buttons()
{
    // Utility Buttons
    connect(ui->save_file, &QPushButton::clicked,
            this, [&]{ save_to_file(); });

    connect(ui->ducky, &QPushButton::clicked,
            this, [&]{ duckytools(gb); });

    connect(ui->partition, &QPushButton::clicked,
            this, [&]{ prepare_ducky(); });

    // Command Buttons - Page 1
    connect(ui->backspace, &QPushButton::clicked,
            this, [&]{ remove_current_line(); });

    connect(ui->new_line, &QPushButton::clicked,
            this, [&]{ insert_newline(); });

    connect(ui->cmd_control, &QPushButton::clicked,
            this, [&]{ insert_command("CONTROL"); });

    connect(ui->cmd_delay, &QPushButton::clicked,
            this, [&]{ insert_command("DELAY"); });

    connect(ui->cmd_down, &QPushButton::clicked,
            this, [&]{ insert_command("DOWN"); });

    connect(ui->cmd_gui, &QPushButton::clicked,
            this, [&]{ insert_command("GUI"); });

    connect(ui->cmd_enter, &QPushButton::clicked,
            this, [&]{ insert_command("ENTER"); });

    connect(ui->cmd_escape, &QPushButton::clicked,
            this, [&]{ insert_command("ESCAPE"); });

    connect(ui->cmd_repeat, &QPushButton::clicked,
            this, [&]{ insert_command("REPEAT"); });

    connect(ui->cmd_string, &QPushButton::clicked,
            this, [&]{ insert_command("STRING"); });

    connect(ui->cmd_tab, &QPushButton::clicked,
            this, [&]{ insert_command("TAB"); });

    connect(ui->cmd_M, &QPushButton::clicked,
            this, [&]{ insert_command("M"); });

    connect(ui->cmd_R, &QPushButton::clicked,
            this, [&]{ insert_command("R"); });

    connect(ui->cmd_s_z, &QPushButton::clicked,
            this, [&]{ cmd_s_z_clicked(); });

    connect(ui->num_100, &QPushButton::clicked,
            this, [&]{ insert_command("100"); });

    connect(ui->num_300, &QPushButton::clicked,
            this, [&]{ insert_command("300"); });

    connect(ui->num_500, &QPushButton::clicked,
            this, [&]{ insert_command("500"); });

    connect(ui->num_2000, &QPushButton::clicked,
            this, [&]{ insert_command("2000"); });

    connect(ui->num_4000, &QPushButton::clicked,
            this, [&]{ insert_command("4000"); });

    connect(ui->num_5000, &QPushButton::clicked,
            this, [&]{ insert_command("5000"); });


    // Command Buttons - Page 2
    connect(ui->cmd_copy_image, &QPushButton::clicked,
            this, [&]{ cmd_copy_image_clicked(); });

    connect(ui->cmd_mount_usb, &QPushButton::clicked,
            this, [&]{ cmd_mount_usb_clicked(); });

    connect(ui->cmd_open_cmdline, &QPushButton::clicked,
            this, [&]{ cmd_open_cmdline_clicked(); });

    connect(ui->cmd_open_image, &QPushButton::clicked,
            this, [&]{ cmd_open_image_clicked(); });

    connect(ui->cmd_alt_space, &QPushButton::clicked,
            this, [&]{ clear_current_line();
                       insert_command("ALT SPACE");
                       insert_newline(); });

    connect(ui->cmd_cd, &QPushButton::clicked,
            this, [&]{ clear_current_line();
                       insert_command("STRING CD %TEMP%");
                       insert_newline(); });


    // Change page of stacked widget
    connect(ui->back_arrow, &QPushButton::clicked,
            this, [&]{ ui->CMD_Widget->setCurrentIndex(0); });

    connect(ui->forward_arrow, &QPushButton::clicked,
            this, [&]{ ui->CMD_Widget->setCurrentIndex(1); });
}


void RubberDucky::cmd_copy_image_clicked()
{
    clear_current_line();

    insert_command("STRING");
    insert_command("ccopy");
    insert_command("%DUCKYdrive%payload-img.png");
    insert_command("%userprofile%\\Desktop\\payload-img.png");

    insert_newline();
}


void RubberDucky::cmd_mount_usb_clicked()
{
    clear_current_line();

    insert_command("STRING");
    insert_command("for /f %d in");
    insert_command("(\'wmic volume get driveletter^, label^|findstr \"DUCKY\"\')");
    insert_command("do @set DUCKYdrive=%d");
    insert_newline();
    insert_command("ENTER");
    insert_newline();
    insert_command("STRING exit");
}


void RubberDucky::cmd_open_cmdline_clicked()
{
    clear_current_line();

    insert_command("STRING");
    insert_command("cmd /Q /D /T:7F /F:OFF /V:ON /K");

    insert_newline();
}


void RubberDucky::cmd_open_image_clicked()
{
    clear_current_line();

    insert_command("STRING");
    insert_command("%USERPROFILE%\\Desktop\\payload-img.png");

    insert_newline();
}


bool swap_char(int *pos, QChar *cmd, const QString &current_line)
{
    if ('s' == current_line.last(1))
    {
        *pos = current_line.lastIndexOf(QChar('s'));
        *cmd = 'z';

        return true;


    }
    else if ('z' == current_line.last(1))
    {
        *pos = current_line.lastIndexOf(QChar('z'));
        *cmd = 's';

        return true;
    }

    return false;
}

void RubberDucky::cmd_s_z_clicked()
{
    int row = ui->script_view->currentIndex().row();
    if (row == -1)
    {
        row = script_model->rowCount()-1;
    }

    QChar cmd = 's';
    if (script_list->at(row).isEmpty())
    {
        insert_command(cmd);

        return;
    }

    // swaps characters 's' and 'z'
    int pos;
    bool swap = swap_char(&pos, &cmd, script_list->at(row));
    if (swap)
    {
        script_list->replace(row, script_list->at(row).left(pos).simplified());

        insert_command(cmd);
    }
    else
    {
        insert_command(cmd);
    }
}


void RubberDucky::insert_command(const QString &cmd)
{
    int row = ui->script_view->currentIndex().row();
    if (row == -1)
    {
        row = script_model->rowCount()-1;
    }

    if (script_list->at(row).isEmpty())
        script_list->replace(row, cmd);
    else
        script_list->replace(row, script_list->at(row) % " " % cmd);

    script_model->setStringList(*script_list);
    ui->script_view->setCurrentIndex(script_model->index(row));

    ui->script_view->setFocus(Qt::ActiveWindowFocusReason);
}


void RubberDucky::insert_newline()
{
    int row = ui->script_view->currentIndex().row()+1;
    if (row == -1)
    {
        row = script_model->rowCount()-1;
    }

    script_list->insert(row, "");
    script_model->setStringList(*script_list);

    ui->script_view->setCurrentIndex(script_model->index(row));

    ui->script_view->setFocus(Qt::ActiveWindowFocusReason);
}


void RubberDucky::remove_current_line()
{
    if (script_model->rowCount() == 1)
    {
        clear_current_line();
        return;
    }

    int row = ui->script_view->currentIndex().row();
    if (row == -1)
    {
        row = script_model->rowCount()-1;
    }

    script_list->removeAt(row);
    script_model->setStringList(*script_list);

    if (row+1 > script_model->rowCount())
    {
        ui->script_view->setCurrentIndex(script_model->index(row-1));
    }
    else
    {
        ui->script_view->setCurrentIndex(script_model->index(row));
    }

        ui->script_view->setFocus(Qt::ActiveWindowFocusReason);
}


void RubberDucky::clear_current_line()
{
    int row = ui->script_view->currentIndex().row();
    if (row == -1)
    {
        row = script_model->rowCount()-1;
    }

    script_list->replace(row, "");

    script_model->setStringList(*script_list);

    ui->script_view->setCurrentIndex(script_model->index(row));
}
