#ifndef DEAUTHER_H
#define DEAUTHER_H

#include <QWidget>

#include "network_model/network_model.h"

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

    NetworkModel *network_model;
    NetworkSort *network_sort;

    void setup_network_list (void);
};

#endif // DEAUTHER_H
