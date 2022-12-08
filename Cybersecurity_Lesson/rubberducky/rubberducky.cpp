#include <QDir>
#include <QErrorMessage>
#include <QFile>
#include <QJsonObject>
#include <QMessageBox>
#include <QScrollBar>
#include <QStringBuilder>
#include <QTextStream>
#include <QToolTip>
#include <QTimer>

#include "languages.h"

#include "rubberducky.h"
#include "ui_rubberducky.h"

RubberDucky::RubberDucky(QWidget *parent) :
    QWidget(parent),
    ui (new Ui::RubberDucky),
    script_model (new QStringListModel (this)),
    script_list (new QStringList ()),
    stylesheets (new CustomStyleSheets)
{
    ui->setupUi(this);

    // Set listview scrollbar colors
    listview_stylesheet = QString(stylesheets->listview_scrollbar());
    ui->script_view->setStyleSheet (listview_stylesheet
                                    % stylesheets->vertical_scrollbar_quirk());

    this->setStyleSheet(rubberducky_stylesheet());

    // Set listview font
    QFont script_font = QFont("DejavuSans", 14, QFont::Normal);
    script_font.setWordSpacing(8);
    ui->script_view->setFont (script_font);

    // install filter to correct horizontal scrollbar quirks
    ui->script_view->horizontalScrollBar()->installEventFilter(this);

    // Set up script window
    script_list->append("");
    script_model->setStringList(*script_list);

    ui->script_view->setModel(script_model);

    setup_cmd_buttons();


    QToolTip::setFont(script_font);

    // Set cmd ui to first page
    ui->CMD_Widget->setCurrentIndex (0);

    // Ensures page is set correctly
    QTimer *timeout = new QTimer();
    timeout->setSingleShot(true);
    timeout->setInterval(500);

    timeout->start();
    while (ui->CMD_Widget->currentIndex() != 0)
    {
        ui->CMD_Widget->setCurrentIndex (0);

        if (!timeout->remainingTime())
        {
            timeout->stop();
            break;
        }
    }
    delete timeout;
}


RubberDucky::~RubberDucky()
{
    delete ui;
}


void RubberDucky::setup_cmd_buttons()
{
    // Utility Buttons
    connect(ui->save_file, &QPushButton::clicked,
            this, [&]{ save_to_file(); });

    connect(ui->ducky, &QPushButton::clicked,
            this, [&]{ duckytools(gb); });

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


QByteArray RubberDucky::encode_script (QStringList &duck_script, QJsonObject &lang)
{
    /* parse script */
    QStringList parsed_line;
    QString cmd, instruction;
    QString cmd_prev, instruction_prev;

    QByteArray encoded_script;
    int default_delay = 0;
    for (QString &line : duck_script)
    {
        if (!(line.size() > 0))
            continue;

        if (line.startsWith("REM") || line.startsWith("rem"))
            continue;

        // preserve previous command
        cmd_prev = cmd;
        instruction_prev = instruction;

        // seperate command and instruction
        parsed_line = line.split(' ');
        if (parsed_line.size() >= 2)
        {
            cmd = line.section(' ', 0, 0);
            instruction = line.section(' ', 1);
        }
        else
        {
            cmd = line.section(' ', 0, 0);
        }

        // parse commands
        bool isInteger = false;
        if (!QString::compare(cmd, "DEFAULT_DELAY")
             || !QString::compare(cmd, "DEFAULTDELAY"))
        {
            default_delay = instruction.toInt(&isInteger);

            if (!isInteger)
                return QByteArray();

            instruction.clear();
            continue;
        }

        int repeat_cnt = 1;
        if (!QString::compare(cmd, "repeat", Qt::CaseInsensitive)
             || !QString::compare(cmd, "replay", Qt::CaseInsensitive))
        {
            repeat_cnt = instruction.toInt(&isInteger);

            if (!isInteger)
                return QByteArray();

            cmd = cmd_prev;
            instruction = instruction_prev;
        }

        QJsonValue json_value;
        QStringList key_string;
        QByteArray key_code;
        QByteArray key_param;
        for (int i=0; i<repeat_cnt; ++i)
        {
            if (!QString::compare(cmd, "STRING"))
            {
                for (QChar &c : instruction)
                {
                    json_value = lang.value(c);
                    key_string = json_value.toString().split(',');

                    if (key_string.isEmpty())
                    {
                        qWarning() << "Warning: Malformed script" << Qt::endl;
                    }

                    for (QString &code : key_string)
                    {
                        key_code.append(code.toUInt(nullptr, 16));
                    }

                    encoded_script.append(key_code.at(2));
                    encoded_script.append(key_code.at(0));

                    key_code.clear();
                }

                instruction.clear();
            }
            else if (!QString::compare(cmd, "DELAY"))
            {
                QString delay;
                uint delay_val = instruction.toUInt();
                while (delay_val > 0)
                {
                    if (delay_val > 255)
                    {
                        delay.append("00FF");

                        delay_val -= 255;
                    }
                    else
                    {
                        delay.append("00");
                        delay.append(QString::number(delay_val,16));
                        delay_val = 0;
                    }
                }

                // retain delay padding (00)
                encoded_script.append(QByteArray::fromHex(delay.toUtf8()));

                instruction.clear();
            }
            else if (lang.keys().contains(cmd))
            {
                json_value = lang.value(cmd);
                key_string = json_value.toString().split(',');

                for (QString &code : key_string)
                {
                    key_code.append(code.toUInt(nullptr, 16));
                }

                if (!instruction.isEmpty())
                {
                    json_value = lang.value(instruction);
                    key_string = json_value.toString().split(',');

                    for (QString &code : key_string)
                    {
                        key_param.append(code.toUInt(nullptr, 16));
                    }
                    key_code[0] |= key_param[0];
                    key_code[2] |= key_param[2];

                    instruction.clear();
                }

                encoded_script.append(key_code.at(2));
                encoded_script.append(key_code.at(0));
            }

            key_code.clear();
            key_param.clear();
        }

        if (default_delay)
        {
            QString delay;
            uint delay_val = default_delay;
            while (delay_val > 0)
            {
                if (delay_val > 255)
                {
                    delay.append("00FF");

                    delay_val -= 255;
                }
                else
                {
                    delay.append("00");
                    delay.append(QString::number(delay_val,16));
                    delay_val = 0;
                }
            }

            // retain delay padding (00)
            encoded_script.append(QByteArray::fromHex(delay.toUtf8()));

            instruction.clear();
        }
    }

    return encoded_script;
}

void RubberDucky::duckytools(QJsonObject &lang)
{
    /* open ducky script file*/
    QString script_path = QDir::homePath() % "/payload.txt";
    QFile duck_file (script_path);
    if (!duck_file.open (QFileDevice::ReadOnly | QFileDevice::Text))
    {
        QMessageBox::warning
        (
            this,
            "File not opened",
            "Error: unable to open file"
        );
    }

    // If the saved script is blank use demo script
    if (duck_file.size() <= 1)
    {
        duck_file.close();

        QString script_path = "/usr/share/cybersecurity_lesson/payload_example.txt";
        //QString script_path = "/home/troytjh/.git_repo/Capstone/build/install/share/cybersecurity_lesson/payload_example.txt";
        duck_file.setFileName(script_path);
        if (!duck_file.open (QFileDevice::ReadOnly | QFileDevice::Text))
        {
            QMessageBox::warning
            (
                this,
                "File not opened",
                "Error: unable to open file"
            );
        }
    }


    /* encode ducky script */
    QTextStream stream (&duck_file);
    QStringList duck_script;
    while (true)
    {
        QString line = stream.readLine();
        if (line.isNull())
            break;
        else
            duck_script.append(line);
    }
    QByteArray payload = encode_script (duck_script, lang);


    /* write encoded ducky script to file */
    QString bin_path = QDir::homePath() % "/payload.bin";
    QFile duck_bin (bin_path);
    if (!duck_bin.open (QFileDevice::WriteOnly))
    {
        QMessageBox::warning
        (
            this,
            "File not opened",
            "Error: unable to open file"
        );
    }
    QDataStream out(&duck_bin);
    out.writeRawData(payload.data(), payload.size());

    duck_file.close();
    duck_bin.close();


    /* display pop-up */
    int offset_x = 20;
    int offset_y = -15;
    QPoint p = ui->tooltip_area->geometry().topLeft();
    p.setX (p.x() + offset_x);
    p.setY (p.y() + offset_y);

    QRect rect = ui->tooltip_area->geometry();

    QToolTip::showText (p, "Saved to\n" % bin_path, this, rect, 3000);
}


void RubberDucky::save_to_file()
{
    QString path = QDir::homePath() % "/payload.txt";
    QFile file (path);

    if (!file.open (QFileDevice::ReadWrite | QFileDevice::Text))
    {
        QMessageBox::warning
        (
            this,
            "File not opened",
            "Error: unable to open file"
        );
    }

    file.resize (0);
    for (int i=0; i < script_model->rowCount(); ++i)
    {
        QTextStream stream (&file);
        stream << script_list->at(i) << Qt::endl;
    }

    int offset_x = 20;
    int offset_y = -15;
    QPoint p = ui->tooltip_area->geometry().topLeft();
    p.setX (p.x() + offset_x);
    p.setY (p.y() + offset_y);

    QRect rect = ui->tooltip_area->geometry();

    QToolTip::showText (p, "Saved to\n" % path, this, rect, 3000);
}


QString RubberDucky::rubberducky_stylesheet()
{
    // Set background color and button font color
    QString stylesheet =
    "QListView { color: rgb(233, 235, 244); } \
     QToolTip { color: rgb(233, 235, 244); }";

    return stylesheet;
}


/* Filter to fix vertical scrollbar border when horizontal scrollbar appears
**
** Add bottom border to vertical scrollbar, on horizontal scrollbar hide
**
** Remove bottom border to vertical scrollbar on horizontal scrollbar appear
**/
bool RubberDucky::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::Hide)
    {
        QString update_scrollbar;

        update_scrollbar =
                listview_stylesheet
                % stylesheets->vertical_scrollbar_quirk();

        ui->script_view->setStyleSheet (update_scrollbar);
    }
    else if (event->type() == QEvent::Show)
    {
        ui->script_view->setStyleSheet (listview_stylesheet);
    }

    return QWidget::eventFilter (object, event);
}

