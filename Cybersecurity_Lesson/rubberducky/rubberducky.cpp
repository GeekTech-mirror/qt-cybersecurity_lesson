#include <Solid/Device>
#include <Solid/DeviceNotifier>
#include <Solid/StorageAccess>
#include <Solid/StorageDrive>

#include <QDir>
#include <QErrorMessage>
#include <QFile>
#include <QFileInfoList>
#include <QJsonObject>
#include <QMessageBox>
#include <QScrollBar>
#include <QScroller>
#include <QStandardPaths>
#include <QStorageInfo>
#include <QStringBuilder>
#include <QTextStream>
#include <QToolTip>
#include <QTimer>

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

    // set search path for documents
    QStringList standard_documentpaths =
            QStandardPaths::standardLocations (
                QStandardPaths::DocumentsLocation
            );
    for (QString &documentpath : standard_documentpaths)
    {
        QDir::addSearchPath("documentpaths", documentpath);
    }

    // set search path for raspberry pi pico setup files
    QDir::addSearchPath("datapaths", CMAKE_DATADIR);   // install data directory
    QDir::addSearchPath("datapaths", "./rubberducky"); // build data directory

    QStringList standard_datapaths =
            QStandardPaths::standardLocations (
                QStandardPaths::AppDataLocation
            );
    for (QString &datapath : standard_datapaths)
    {
        QDir::addSearchPath("datapaths", datapath);
    }


    // Set listview scrollbar colors
    listview_stylesheet = QString(stylesheets->listview_scrollbar());
    ui->script_view->setStyleSheet (listview_stylesheet
                                    % stylesheets->vertical_scrollbar_quirk());

    this->setStyleSheet(rubberducky_stylesheet());

    // Set listview font
    QFont script_font = QFont("DejavuSans", 14, QFont::Normal);
    script_font.setWordSpacing(8);

    QToolTip::setFont(script_font);
    ui->script_view->setFont (script_font);

    // install filter to correct horizontal scrollbar quirks
    ui->script_view->horizontalScrollBar()->installEventFilter(this);

    // Set up script window
    script_list->append("");
    script_model->setStringList(*script_list);

    ui->script_view->setModel(script_model);

    QScroller::grabGesture(ui->script_view, QScroller::TouchGesture);

    // setup command buttons
    setup_cmd_buttons();

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


void RubberDucky::save_to_file()
{
    QString scriptpath;
    QStringList documentpaths = QDir::searchPaths("documentpaths");
    for (QString &path : documentpaths)
    {
        QDir dir(path);
        if (dir.exists())
        {
            scriptpath = dir.absoluteFilePath("payload.txt");
            break;
        }
    }

    // QString path =  QDir::homePath() % "/payload.txt";
    QFile script_file (scriptpath);

    if (!script_file.open (QFileDevice::ReadWrite | QFileDevice::Text))
    {
        QMessageBox::warning
        (
            this,
            "File not opened",
            "Error: unable to open file"
        );
    }

    script_file.resize (0);
    for (int i=0; i < script_model->rowCount(); ++i)
    {
        QTextStream stream (&script_file);
        stream << script_list->at(i) << Qt::endl;
    }

    int offset_x = 20;
    int offset_y = -15;
    QPoint p = ui->tooltip_area->geometry().topLeft();
    p.setX (p.x() + offset_x);
    p.setY (p.y() + offset_y);

    QRect rect = ui->tooltip_area->geometry();

    QToolTip::showText (p, "Saved to\n" % scriptpath, this, rect, 3000);
}


void RubberDucky::prepare_ducky()
{
    //QStorageInfo storage = QStorageInfo::root();
    //qDebug() << storage.rootPath();
    //qDebug() << QStorageInfo::mountedVolumes() << Qt::endl;

    /* locate pico setup files */
    QString srcpath;
    QStringList datapaths = QDir::searchPaths("datapaths");
    for (QString &path : datapaths)
    {
        QDir dir(path);
        if (dir.exists("pico_files"))
        {
            srcpath = dir.absoluteFilePath("pico_files");
            break;
        }
    }

    if (!QDir(srcpath).exists())
    {
        qWarning() << "Warning: pico setup files missing";
        qWarning() << "Skipping pico setup" << Qt::endl;

    }

    /* mount pico and copy source path to pico */
    mount_pico(srcpath);
}

void RubberDucky::mount_pico(QString srcpath)
{
    /* mount usb */
    QList<Solid::Device> devices =
            Solid::Device::listFromType (
                Solid::DeviceInterface::StorageAccess
            );
    for (Solid::Device &device : devices)
    {
        if (!device.is<Solid::StorageAccess>())
            continue;

        if (!device.displayName().contains("RPI-RP2"))
            continue;

        qDebug() << "Storage Drive: " << device.displayName();

        /* mount raspberry pi pico */
        Solid::StorageAccess *access = device.as<Solid::StorageAccess>();

        QString pico_mnt = access->filePath();
        if (pico_mnt.isEmpty())
        {
            access->setup();

            // copy files after mounting is done
            connect(access,
                    &Solid::StorageAccess::setupDone,
                    this,
                    [&, srcpath](Solid::ErrorType error)
                    { copy_to_pico(srcpath, sender(), error); });

            return;
        }

        /* pico is already mounted */
        /* copy ducky files to raspberry pi pico */
        if (!QDir(pico_mnt).exists()
            || pico_mnt.isEmpty())
        {
            qWarning() << "Warning: no device mounted";
            qWarning() << "Skipping pico setup" << Qt::endl;
            return;
        }

        copy_folder (srcpath, pico_mnt);
    }
}

void RubberDucky::copy_to_pico (QString srcpath, QObject *sender, Solid::ErrorType error)
{
    /* find storage device from signal */
    Solid::StorageAccess *access =
            qobject_cast<Solid::StorageAccess *>(sender);
    QString pico_mnt = access->filePath();
    if (error)
    {
        qWarning() << "Warning: mounting the Raspberry Pi Pico failed";
        qWarning() << "Skipping pico setup" << Qt::endl;

        return;
    }

    copy_folder(srcpath, pico_mnt);
}



void RubberDucky::copy_folder (QString srcpath, QString destpath)
{
    /* copy payload */
    QFileInfo src_info(srcpath);
    if (src_info.isFile()
        && QDir(destpath).exists())
    {
        QString src = srcpath;
        QString dest = destpath + "/" + "payload.dd";
        if (QFile::exists(dest))
        {
            QFile::remove(dest);
        }

        QFile::copy (src, dest);

        return;
    }


    /* copy directory */
    QDir srcdir(srcpath);
    QDir destdir(destpath);

    if (!srcdir.exists())
    {
        qWarning() << "Warning: source files do not exist";
        qWarning() << "No files to copy" << Qt::endl;

        return;
    }

    if (!destdir.exists())
    {
        destdir.mkpath(destpath);
    }


    QFileInfoList entries = srcdir.entryInfoList (QDir::NoDotAndDotDot
                                                  | QDir::Files
                                                  | QDir::Dirs);
    for (const QFileInfo &entry : entries)
    {
        QString src = entry.filePath();
        QString dest = destpath + "/" + entry.fileName();

        if (entry.isDir())
        {
            copy_folder (src, dest);
        }
        else
        {
            if (QFile::exists(dest))
            {
                QFile::remove(dest);
            }

            QFile::copy (src, dest);
        }
    }

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

