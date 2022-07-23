#ifndef RUBBERDUCKY_H
#define RUBBERDUCKY_H

#include <QWidget>

namespace Ui {
class RubberDucky;
}

class RubberDucky : public QWidget
{
    Q_OBJECT

public:
    explicit RubberDucky(QWidget *parent = nullptr);
    ~RubberDucky();

private:
    Ui::RubberDucky *ui;
};

#endif // RUBBERDUCKY_H
