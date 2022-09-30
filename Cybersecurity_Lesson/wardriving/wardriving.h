#ifndef WARDRIVING_H
#define WARDRIVING_H

/* Qt include files */
#include <QWidget>

/* local include files */
#include "custom_stylesheets.h"
#include "network_model/network_model.h"
#include "network_model/network_sort.h"

namespace Ui {
class WarDriving;
}

class WarDriving : public QWidget
{
    Q_OBJECT

public:
    explicit WarDriving(QWidget *parent = nullptr);
    ~WarDriving();

private:
    Ui::WarDriving *ui;

    CustomStyleSheets *stylesheets;
    QString treeview_stylesheet;

    NetworkModel *network_model;
    NetworkSort *network_sort;

    void setup_network_view (void);


protected:
    bool eventFilter (QObject *object, QEvent *event) override;
};

#endif // WARDRIVING_H
