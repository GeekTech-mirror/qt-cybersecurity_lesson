#ifndef DEAUTHER_H
#define DEAUTHER_H

#include <QPropertyAnimation>
#include <QPushButton>
#include <QTimer>
#include <QWidget>

#include "station_model.h"
#include "network_model.h"
#include "iface_model.h"

namespace Ui {
class Deauther;
}

class Deauther : public QWidget
{
    Q_OBJECT

public:
    explicit Deauther(QWidget *parent = nullptr);
    ~Deauther();

private:
    Ui::Deauther *ui;

    StationModel *station_model;
    IfaceModel *iface_model;

    NetworkModel *network_model;
    NetworkSort *network_sort;

    QTimer *search_timer;
    uint8_t search_animation_state = 1;

    void setup_network_list (void);

    void toggle_monitoring ();

    void search_animation (QPushButton *button);
};

#endif // DEAUTHER_H
