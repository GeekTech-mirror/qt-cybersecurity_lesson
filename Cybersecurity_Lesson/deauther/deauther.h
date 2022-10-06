#ifndef DEAUTHER_H
#define DEAUTHER_H

#include <QPropertyAnimation>
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

    QPropertyAnimation *search_animation;
    QTimer *search_timer;

    void setup_network_list (void);
};

#endif // DEAUTHER_H
