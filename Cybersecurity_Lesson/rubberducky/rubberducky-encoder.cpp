#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QStringBuilder>
#include <QToolTip>

#include "rubberducky.h"
#include "ui_rubberducky.h"


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
    QString script_path;
    QString bin_path;
    QStringList documentpaths = QDir::searchPaths("documentpaths");
    for (QString &path : documentpaths)
    {
        QDir dir(path);
        if (dir.exists())
        {
            script_path = dir.absoluteFilePath("payload.txt");
            bin_path = dir.absoluteFilePath("payload.dd");
            break;
        }
    }

    /* open ducky script file*/
    //QString script_path = QDir::homePath() % "/payload.txt";
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

    /* if the saved script is blank use example script */
    if (duck_file.size() <= 1)
    {
        duck_file.close();

        /* locate example payload */
        QStringList datapaths = QDir::searchPaths("datapaths");
        for (QString &path : datapaths)
        {
            QDir dir(path);
            if (dir.exists("payload_example.txt"))
            {
                script_path = dir.absoluteFilePath("payload_example.txt");
                duck_file.setFileName(script_path);
                break;
            }
        }

        //QString script_path = "/usr/share/cybersecurity_lesson/payload_example.txt";
        //QString script_path = "/home/troytjh/.git_repo/Capstone/build/install/share/cybersecurity_lesson/payload_example.txt";
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
    //QString bin_path = duck_script. % "/payload.bin";
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


    mount_pico(script_path);

    /* display pop-up */
    int offset_x = 20;
    int offset_y = -15;
    QPoint p = ui->tooltip_area->geometry().topLeft();
    p.setX (p.x() + offset_x);
    p.setY (p.y() + offset_y);

    QRect rect = ui->tooltip_area->geometry();

    QToolTip::showText (p, "Saved to\n" % bin_path, this, rect, 3000);
}
