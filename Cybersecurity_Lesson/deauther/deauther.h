#ifndef DEAUTHER_H
#define DEAUTHER_H

#include <QPropertyAnimation>
#include <QPushButton>
#include <QTimer>
#include <QWidget>

#include "pcap.h"

#include "station_model.h"
#include "network_model.h"
#include "iface_model.h"

#include "custom_stylesheets.h"

namespace Ui {
class Deauther;
}

class Deauther : public QWidget
{
    Q_OBJECT

public:
    explicit Deauther(QWidget *parent = nullptr);
    ~Deauther();

    pcap_t *iface_handle;

private:
    Ui::Deauther *ui;

    QMutex station_mutex;

    CustomStyleSheets *stylesheets;
    QString treeview_stylesheet;

    StationModel *station_model;
    IfaceModel *iface_model;

    NetworkModel *network_model;
    NetworkSort *network_sort;

    QTimer *search_animation_timer;
    uint8_t search_animation_state = 1;

    QByteArray deauther_packet;


    void setup_network_list (void);
    void setup_station_view (void);

    void toggle_monitoring ();

    void search_animation (QPushButton *button);

    void deauther_attack ();

protected:
    bool eventFilter (QObject *object, QEvent *event) override;
};

#endif // DEAUTHER_H
