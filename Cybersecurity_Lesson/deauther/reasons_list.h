#ifndef REASONS_LIST_H
#define REASONS_LIST_H

#include <QComboBox>
#include <QtGui>
#include <QItemDelegate>
#include <QListView>
#include <QMetaType>
#include <QPainter>
#include <QStyle>
#include <QStyledItemDelegate>
#include <QWidget>

#include "custom_stylesheets.h"


// Handle word wrap and pop-up size for Deauth Resons
// QComboBox
class ReasonsDelegate : public QStyledItemDelegate
{
public:
    ReasonsDelegate(QObject* parent)
    : QStyledItemDelegate(parent) {}

    void paint (QPainter *painter,
                const QStyleOptionViewItem &option,
                const QModelIndex &index) const
    {
        int widthUsed, lineCount;

        //Try and word wrap strings
        if(index.data().canConvert(QMetaType::QString))
        {
            painter->save();

            QPalette::ColorGroup group = option.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
            if (group == QPalette::Normal && !(option.state & QStyle::State_Active))
                group = QPalette::Inactive;

            //set pen color depending on behavior
            painter->setFont( QFont("Sans Serif", 14, QFont::Normal) );
            if (option.state & QStyle::State_Selected)
                painter->setPen(option.palette.color(group, QPalette::HighlightedText));
            else
                painter->setPen(option.palette.color(group, QPalette::Text));

            //Text from item
            QString text = index.data(Qt::DisplayRole).toString();

            //Begin word-wrapping effect (use the provided rectangles width to determine when to wrap)
            details(text, QFont("Sans Serif", 14, QFont::Normal), option, &lineCount, &widthUsed);

            //Word wrap the text, 'elide' it if it goes past a pre-determined maximum
            QString newText = painter->fontMetrics().elidedText(text, Qt::ElideRight, widthUsed);
            qDebug() << option.rect.bottomRight();
            painter->drawText( option.rect, (Qt::TextWrapAnywhere|Qt::TextWordWrap|Qt::AlignTop|Qt::AlignLeft), newText );
            painter->restore();
        }
        else
        {
            //Fall back on original
            QStyledItemDelegate::paint(painter, option, index);
        }

    }

    QSize sizeHint (const QStyleOptionViewItem &option,
                    const QModelIndex &index) const
    {
        int widthUsed, lineCount;

        const QWidget *editor = option.widget;


        //Try and word wrap strings
        if(index.data().canConvert(QMetaType::QString))
        {
            //Update the size based on the number of lines (original size of a single line multiplied
            //by the number of lines)
            QString text = index.data(Qt::DisplayRole).toString();
            details(text, QFont("Sans Serif", 14, QFont::Normal), option, &lineCount, &widthUsed);

            QSize size = QStyledItemDelegate::sizeHint(option, index);
            //size.setHeight(lineCount *  size.height());

            if (size.width() > option.widget->maximumWidth())
            {
                //qDebug() << size;
                size.setWidth (option.widget->maximumWidth());
            }

            return size;
        }
        else
        {
            //Fall back on original size hint of item
            return QStyledItemDelegate::sizeHint(option, index);
        }

    }

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index ) const
    {
        int lineCount, widthUsed;

        //Fix editor's geometry and produce the word-wrapped effect for strings...
        if(index.data().canConvert(QMetaType::QString))
        {
            //We need to expand our editor accordingly (not really necessary but looks nice)
            QString text = index.data(Qt::DisplayRole).toString();
            details(text, QFont("Sans Serif", 14, QFont::Normal), option, &lineCount, &widthUsed);

            //Expand
            QSize extraSize = QSize(option.rect.width(), lineCount * option.rect.height());
            QRect extraRect = option.rect;
            extraRect.setSize(extraSize);
            editor->setGeometry(option.rect);
        }
        else
        {
            //Fall back on original update
            QStyledItemDelegate::updateEditorGeometry(editor, option, index);
        }

    }

private:
    void details( QString text, const QFont font,
                  const QStyleOptionViewItem &option, int *lineCount, int *widthUsed ) const
    {
        //Use text layout to word-wrap and provide information about line counts and width's
        QTextLayout textLayout(text);

        *widthUsed = 0;
        *lineCount = 0;
        textLayout.setFont(font);
        textLayout.beginLayout();
        while (*lineCount < 5)
        {
            *lineCount = *lineCount + 1;
            QTextLine line = textLayout.createLine();
            if (!line.isValid())
                break;

            line.setLineWidth(option.rect.width());
            *widthUsed = (*widthUsed + line.naturalTextWidth());
        }
        textLayout.endLayout();
        *widthUsed = (*widthUsed + option.rect.width());

    }

};


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

//        int lineCount = 10;
//        QSize extraSize = QSize(this->rect().width(), lineCount * this->rect().height());
//        QRect extraRect = QRect(this->rect().topLeft(), extraSize);
//        //extraRect.setSize(extraSize);
//        list_view->setFrameRect(extraRect);
//        list_view->viewport().rec
        //list_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        //list_view->setItemDelegate(new ReasonsDelegate (ui->reasons_list));
        //list_view->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        //list_view->setSizePolicy (QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        //list_view->setTextElideMode(Qt::ElideNone);
        //list_view->window()->setFixedWidth(444);

        //view()->setMaximumWidth(444);
        //view()->setMaximumHeight(800);
        //QPoint p = QPoint(ui->reasons_list->view()->rect().topLeft());
        //QRect r(p, QSize(500,600));
        //ui->reasons_list->view()->adjustSize();
        //ui->reasons_list->view()->
        this->setView(list_view);
        //view()->setItemDelegate(new ReasonsDelegate (this));
        //view()->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
        //ui->reasons_list->view()->setMaximumHeight(600);
        //this->setItemDelegate(new ReasonsDelegate (this));
//        this->setStyleSheet(
//        "QComboBox QAbstractItemView { \
//            background-color: rgb(100,0,0); \
//            color rgb (100, 0, 0); \
//            min-width: 444; \
//        }"
//                    );

        //QWidget w = QWidget ()

        //this->view()->setTextElideMode(Qt::ElideRight);
        //qDebug() << this->view()->viewport()->rect().bottomRight();
    }

    void showPopup() override
    {
        QPoint p = QPoint(rect().topLeft());
        QRect r(p, QSize(444,444));
        update(r);

        this->view()->setMaximumWidth(444);

        this->setMaximumSize(444,444);
        //qDebug() << this->frameSize();

        //qDebug() << "coord" << this->view()->rect().size();

        QComboBox::showPopup();
    }

private:
    CustomStyleSheets *stylesheets;
};

#endif // REASONS_LIST_H
