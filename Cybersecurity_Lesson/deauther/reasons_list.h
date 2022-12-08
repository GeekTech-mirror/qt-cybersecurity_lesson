#ifndef REASONS_LIST_H
#define REASONS_LIST_H

#include <QComboBox>
#include <QtGui>
#include <QItemDelegate>
#include <QListView>
#include <QPainter>
#include <QStyle>
#include <QWidget>

#include "custom_stylesheets.h"


// Handle word wrap and pop-up size for Deauth Reasons
class ReasonsList : public QComboBox
{
    Q_OBJECT

public:
    ReasonsList(QWidget* parent  = nullptr) :
        QComboBox(parent),
        stylesheets (new CustomStyleSheets)
    {
        //ui->reasons_list = ReasonsList();

        addItem ("0x01 - Unspecified reason");

        addItem ("0x02 - Previous authentication no longer valid");

        addItem ("0x03 - Deauthenticated because sending station "
                 "is leaving");

        addItem ("0x04 - Disassociated due to inactivity");

        addItem ("0x05 - Disassociated because WAP device is unable "
                 "to handle currently associated STAs");

        addItem ("0x06 - Class 2 frame received from "
                 "nonauthenticated STA");

        addItem ("0x07 - Class 3 frame received from nonassociated STA");

        addItem ("0x08 - Disassociated because sending STA is "
                 "leaving or has left Basic Service Set (BSS)");

        addItem ("0x09 - STA requesting (re)association is not "
                 "authenticated with responding STA");

        addItem ("0x0a - Disassociated because the information "
                 "in the Supported Channels element is "
                 "unacceptable");

        addItem ("0x0b - Disassociated because the information "
                 "in the Supported Channels element is "
                 "unacceptable");

        addItem ("0x0c - Disassociated due to BSS Transition Management");

        addItem ("0x0d - Invalid element, that is, an element defined "
                 "in this standard for which the content does "
                 "not meet the specifications in Clause 8");

        addItem ("0x0e - Message integrity code (MIC) failure");

        addItem ("0x0f - 4-Way Handshake timeout");

        addItem ("0x10 - Group Key Handshake timeout");

        addItem ("0x11 - Element in 4-Way Handshake different from "
                 "(Re)Association Request/Probe Request/Becon Frame");

        addItem ("0x12 - Invalid group cipher");

        addItem ("0x13 - Invalid pairwise cipher");

        addItem ("0x14 - Invalid AKMP");

        addItem ("0x15 - Unsupported RSNE version");

        addItem ("0x16 - Invalid RSNE capabilities");

        addItem ("0x17 - IEEE 802.1X authentication failed");

        addItem ("0x18 - Cipher suite rejected because of the security policy");


        // add word wrap to drop down list
        QListView *list_view = new QListView();
        list_view->setWordWrap(true);
        //list_view->setGridSize(QSize(50,40));
        list_view->setMaximumWidth(444);
        list_view->setMaximumHeight(800);
        list_view->setFont (QFont("Sans Serif", 14, QFont::Normal));
        list_view->setStyleSheet(stylesheets->combobox_dropdown());

        this->setView(list_view);
    }

    void showPopup() override
    {
        QPoint p = QPoint(rect().topLeft());
        QRect r(p, QSize(444,444));
        update(r);

        this->view()->setMaximumWidth(444);
        this->setMaximumSize(444,444);
        //view()->parentWidget()->setWindowOpacity(0);

        //inputMethodEvent()
        QComboBox::showPopup();
    }

private:
    CustomStyleSheets *stylesheets;
};

#endif // REASONS_LIST_H
