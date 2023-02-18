#ifndef REASONS_LIST_H
#define REASONS_LIST_H

#include <QComboBox>
#include <QtGui>
#include <QItemDelegate>
#include <QListView>
#include <QPainter>
#include <QScrollBar>
#include <QScroller>
#include <QStyle>
#include <QWidget>

#include "custom_stylesheets.h"


// Handle word wrap and pop-up size for Deauth Reasons
class CustomComboBox : public QComboBox
{
    Q_OBJECT

public:
    CustomComboBox(QWidget* parent  = nullptr) :
        QComboBox(parent),
        stylesheets (new CustomStyleSheets)
    {
        max_w = parent->width();
        max_h = parent->height();

        // add word wrap to drop down list
        QListView *list_view = new QListView();
        list_view->setWordWrap(true);
        list_view->setMaximumWidth(max_w);
        list_view->setMaximumHeight(max_h);
        list_view->setFont (QFont("Sans Serif", 14, QFont::Normal));
        list_view->setStyleSheet(stylesheets->combobox_popup());

        list_view->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        QScroller::grabGesture(list_view, QScroller::TouchGesture);

        this->setView(list_view);
    }

    void showPopup() override
    {
        int cb_y = this->mapToParent(this->rect().topLeft()).y();
        int popup_h = max_h - cb_y - popup_offset_y;  // stop before the tool bar
        int popup_w = this->width();                  // use combobox width

        // reset list to open from beginning
        this->setCurrentIndex(0);

        // generate popup frame
        QComboBox::showPopup();

        // adjust popup frame to text width
        QWidget *popup = this->findChild<QFrame*>();

        popup->setMaximumWidth(popup_w);
        popup->setMaximumHeight(popup_h);

        qDebug() << popup->height() << popup->width();

        //QScroller::grabGesture(popup, QScroller::LeftMouseButtonGesture);

        int popup_y = this->mapToGlobal(this->rect().topLeft()).y();
        popup->move(popup->x(), popup_y);
        //popup->resize (QSize(444, 660));
    }

private:
    CustomStyleSheets *stylesheets;

    int max_w;// = 444;
    int max_h;//= 724;

    int popup_offset_y = 10;

};

#endif // REASONS_LIST_H
