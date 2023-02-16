/* RubberDucky Attack
** File: rubberducky.cpp
** --------
** Encode ducky script for hack5 bad usb
**
** Move payload to bad usb
** --------
*/

/* Qt include files */
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QStringBuilder>
#include <QToolTip>

/* local includes*/
#include "rubberducky.h"
#include "ui_rubberducky.h"


QByteArray RubberDucky::encode_script (QStringList &duck_script, QJsonObject &lang)
{
    /* parse script */
    QByteArray encoded_script;
    int default_delay = 0;

    QStringList parsed_line;
    QString cmd, instruction;
    QString cmd_prev, instruction_prev;
    for (QString &line : duck_script)
    {
        parsed_line = line.split(' ');

        if (!(line.size() > 0))
            continue;

        if (line.startsWith("REM", Qt::CaseInsensitive))
        {
            continue;
        }

        // preserve previous command
        cmd_prev = cmd;
        instruction_prev = instruction;

        // seperate command and instruction
        cmd = parsed_line.takeFirst();
        instruction = parsed_line.join(' ');


        qDebug() << cmd << instruction;

        if (cmd.startsWith("DEFAULT_DELAY", Qt::CaseInsensitive)
            || cmd.startsWith("DEFAULTDELAY", Qt::CaseInsensitive))
        {

            bool isInt = false;
            default_delay = instruction.toInt(&isInt);

            if (!isInt)
            {
                return QByteArray();
            }

            continue;
        }
        else if (cmd.startsWith("REPEAT", Qt::CaseInsensitive)
                 || cmd.startsWith("REPLAY", Qt::CaseInsensitive))
        {
            bool isInt = false;
            int repeat_cnt = instruction.toInt(&isInt);

            if (!isInt)
            {
                return QByteArray();
            }

            if (repeat_cnt == 0)
            {
                repeat_cnt = 1;
            }

            cmd = cmd_prev;
            instruction = instruction_prev;

            for (int i=0; i<repeat_cnt; ++i)
            {
                parse_instruction (cmd, instruction, &encoded_script, lang);
            }
        }
        else
        {
            parse_instruction(cmd, instruction, &encoded_script, lang);
        }


        if (default_delay)
        {
            encoded_script.append(delay_bytes(default_delay));
        }
    }

    return encoded_script;
}


void  RubberDucky::parse_instruction (const QString &cmd,
                                      const QString &instruction,
                                      QByteArray *encoded_script,
                                      const QJsonObject& lang)
{
    static QJsonValue json_value;
    static QStringList key_string;
    static QByteArray key_code, key_param;

    if (!QString::compare(cmd, "STRING"))
    {
        for (const QChar &c : instruction)
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

            encoded_script->append(key_code.at(2));
            encoded_script->append(key_code.at(0));

            key_code.clear();
        }
    }
    else if (!QString::compare(cmd, "DELAY"))
    {
        encoded_script->append(delay_bytes(instruction.toUInt()));
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
        }

        encoded_script->append(key_code.at(2));
        encoded_script->append(key_code.at(0));
    }

    key_code.clear();
    key_param.clear();
}


QByteArray RubberDucky::delay_bytes(uint delay)
{
    QString delay_bytes;
    while (delay > 0)
    {
        if (delay > 255)
        {
            delay_bytes.append("00FF");

            delay -= 255;
        }
        else
        {
            delay_bytes.append("00");
            delay_bytes.append(QString::number(delay,16));
            delay = 0;
        }
    }

    return QByteArray::fromHex(delay_bytes.toUtf8());
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
