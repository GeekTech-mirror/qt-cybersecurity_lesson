/********************************************************************************
** Form generated from reading UI file 'main_window.ui'
**
** Created by: Qt User Interface Compiler version 6.2.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAIN_WINDOW_H
#define UI_MAIN_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Main_Window
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QSpacerItem *horizontalSpacer_6;
    QLabel *label;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *push_button_1;
    QSpacerItem *horizontalSpacer;
    QPushButton *push_button_2;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *push_button_3;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *push_button_4;
    QToolBar *toolBar;

    void setupUi(QMainWindow *Main_Window)
    {
        if (Main_Window->objectName().isEmpty())
            Main_Window->setObjectName(QString::fromUtf8("Main_Window"));
        Main_Window->resize(480, 800);
        Main_Window->setMinimumSize(QSize(480, 800));
        Main_Window->setMaximumSize(QSize(480, 800));
        QPalette palette;
        QBrush brush(QColor(30, 34, 39, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        Main_Window->setPalette(palette);
        Main_Window->setStyleSheet(QString::fromUtf8("background-color: rgb(30, 34, 39);\n"
"\n"
"QPushButton {\n"
"	background-color: rgb(37, 41, 44);\n"
"}"));
        centralwidget = new QWidget(Main_Window);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        centralwidget->setEnabled(true);
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::Button, brush);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush);
        centralwidget->setPalette(palette1);
        centralwidget->setStyleSheet(QString::fromUtf8("background-color: rgb(30, 34, 39);"));
        verticalLayout_2 = new QVBoxLayout(centralwidget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(20, -1, 20, 30);
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer_6);

        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(36);
        font.setBold(true);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("color: rgb(218, 218, 218);"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setFont(font);
        label_2->setStyleSheet(QString::fromUtf8("color: rgb(218, 218, 218);"));
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_2);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer_7);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer_4);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer_5);

        push_button_1 = new QPushButton(centralwidget);
        push_button_1->setObjectName(QString::fromUtf8("push_button_1"));
        push_button_1->setEnabled(true);
        push_button_1->setMinimumSize(QSize(419, 110));
        QPalette palette2;
        QBrush brush1(QColor(255, 255, 255, 210));
        brush1.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush1);
        palette2.setBrush(QPalette::Active, QPalette::Button, brush);
        QBrush brush2(QColor(24, 27, 29, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(37, 41, 44, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(31, 31, 31, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(175, 175, 175, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Mid, brush5);
        QBrush brush6(QColor(218, 218, 218, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Text, brush6);
        QBrush brush7(QColor(255, 51, 51, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::BrightText, brush7);
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush);
        palette2.setBrush(QPalette::Active, QPalette::Window, brush);
        QBrush brush8(QColor(118, 118, 118, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Shadow, brush8);
        QBrush brush9(QColor(31, 117, 204, 170));
        brush9.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Highlight, brush9);
        QBrush brush10(QColor(255, 255, 255, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::HighlightedText, brush10);
        QBrush brush11(QColor(0, 122, 244, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::Link, brush11);
        QBrush brush12(QColor(165, 122, 255, 255));
        brush12.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::LinkVisited, brush12);
        QBrush brush13(QColor(53, 54, 55, 255));
        brush13.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::AlternateBase, brush13);
        QBrush brush14(QColor(0, 0, 0, 255));
        brush14.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::NoRole, brush14);
        QBrush brush15(QColor(17, 17, 17, 255));
        brush15.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::ToolTipBase, brush15);
        palette2.setBrush(QPalette::Active, QPalette::ToolTipText, brush6);
        QBrush brush16(QColor(128, 128, 129, 255));
        brush16.setStyle(Qt::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette2.setBrush(QPalette::Active, QPalette::PlaceholderText, brush16);
#endif
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush6);
        palette2.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette2.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette2.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush6);
        palette2.setBrush(QPalette::Inactive, QPalette::BrightText, brush7);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush6);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Shadow, brush8);
        palette2.setBrush(QPalette::Inactive, QPalette::Highlight, brush9);
        palette2.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush10);
        palette2.setBrush(QPalette::Inactive, QPalette::Link, brush11);
        palette2.setBrush(QPalette::Inactive, QPalette::LinkVisited, brush12);
        palette2.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush13);
        QBrush brush17(QColor(0, 0, 0, 255));
        brush17.setStyle(Qt::NoBrush);
        palette2.setBrush(QPalette::Inactive, QPalette::NoRole, brush17);
        palette2.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush15);
        palette2.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush6);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette2.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush16);
#endif
        QBrush brush18(QColor(164, 166, 168, 96));
        brush18.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush18);
        palette2.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette2.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette2.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette2.setBrush(QPalette::Disabled, QPalette::Text, brush18);
        palette2.setBrush(QPalette::Disabled, QPalette::BrightText, brush7);
        palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush18);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::Window, brush);
        QBrush brush19(QColor(177, 177, 177, 255));
        brush19.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Disabled, QPalette::Shadow, brush19);
        palette2.setBrush(QPalette::Disabled, QPalette::Highlight, brush9);
        palette2.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush10);
        palette2.setBrush(QPalette::Disabled, QPalette::Link, brush11);
        palette2.setBrush(QPalette::Disabled, QPalette::LinkVisited, brush12);
        palette2.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush13);
        QBrush brush20(QColor(0, 0, 0, 255));
        brush20.setStyle(Qt::NoBrush);
        palette2.setBrush(QPalette::Disabled, QPalette::NoRole, brush20);
        palette2.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush15);
        palette2.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush6);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette2.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush16);
#endif
        push_button_1->setPalette(palette2);
        QFont font1;
        font1.setPointSize(26);
        font1.setBold(true);
        push_button_1->setFont(font1);

        verticalLayout->addWidget(push_button_1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer);

        push_button_2 = new QPushButton(centralwidget);
        push_button_2->setObjectName(QString::fromUtf8("push_button_2"));
        push_button_2->setMinimumSize(QSize(419, 110));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush1);
        palette3.setBrush(QPalette::Active, QPalette::Button, brush);
        palette3.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette3.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        palette3.setBrush(QPalette::Active, QPalette::Dark, brush4);
        palette3.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette3.setBrush(QPalette::Active, QPalette::Text, brush6);
        palette3.setBrush(QPalette::Active, QPalette::BrightText, brush7);
        palette3.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush);
        palette3.setBrush(QPalette::Active, QPalette::Window, brush);
        palette3.setBrush(QPalette::Active, QPalette::Shadow, brush8);
        palette3.setBrush(QPalette::Active, QPalette::Highlight, brush9);
        palette3.setBrush(QPalette::Active, QPalette::HighlightedText, brush10);
        palette3.setBrush(QPalette::Active, QPalette::Link, brush11);
        palette3.setBrush(QPalette::Active, QPalette::LinkVisited, brush12);
        palette3.setBrush(QPalette::Active, QPalette::AlternateBase, brush13);
        palette3.setBrush(QPalette::Active, QPalette::NoRole, brush14);
        palette3.setBrush(QPalette::Active, QPalette::ToolTipBase, brush15);
        palette3.setBrush(QPalette::Active, QPalette::ToolTipText, brush6);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette3.setBrush(QPalette::Active, QPalette::PlaceholderText, brush16);
#endif
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush6);
        palette3.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette3.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette3.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette3.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette3.setBrush(QPalette::Inactive, QPalette::Text, brush6);
        palette3.setBrush(QPalette::Inactive, QPalette::BrightText, brush7);
        palette3.setBrush(QPalette::Inactive, QPalette::ButtonText, brush6);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::Shadow, brush8);
        palette3.setBrush(QPalette::Inactive, QPalette::Highlight, brush9);
        palette3.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush10);
        palette3.setBrush(QPalette::Inactive, QPalette::Link, brush11);
        palette3.setBrush(QPalette::Inactive, QPalette::LinkVisited, brush12);
        palette3.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush13);
        QBrush brush21(QColor(0, 0, 0, 255));
        brush21.setStyle(Qt::NoBrush);
        palette3.setBrush(QPalette::Inactive, QPalette::NoRole, brush21);
        palette3.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush15);
        palette3.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush6);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette3.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush16);
#endif
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush18);
        palette3.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette3.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette3.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette3.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette3.setBrush(QPalette::Disabled, QPalette::Text, brush18);
        palette3.setBrush(QPalette::Disabled, QPalette::BrightText, brush7);
        palette3.setBrush(QPalette::Disabled, QPalette::ButtonText, brush18);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Window, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::Shadow, brush19);
        palette3.setBrush(QPalette::Disabled, QPalette::Highlight, brush9);
        palette3.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush10);
        palette3.setBrush(QPalette::Disabled, QPalette::Link, brush11);
        palette3.setBrush(QPalette::Disabled, QPalette::LinkVisited, brush12);
        palette3.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush13);
        QBrush brush22(QColor(0, 0, 0, 255));
        brush22.setStyle(Qt::NoBrush);
        palette3.setBrush(QPalette::Disabled, QPalette::NoRole, brush22);
        palette3.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush15);
        palette3.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush6);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette3.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush16);
#endif
        push_button_2->setPalette(palette3);
        push_button_2->setFont(font1);

        verticalLayout->addWidget(push_button_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer_2);

        push_button_3 = new QPushButton(centralwidget);
        push_button_3->setObjectName(QString::fromUtf8("push_button_3"));
        push_button_3->setMinimumSize(QSize(419, 110));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush1);
        palette4.setBrush(QPalette::Active, QPalette::Button, brush);
        palette4.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette4.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        palette4.setBrush(QPalette::Active, QPalette::Dark, brush4);
        palette4.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette4.setBrush(QPalette::Active, QPalette::Text, brush6);
        palette4.setBrush(QPalette::Active, QPalette::BrightText, brush7);
        palette4.setBrush(QPalette::Active, QPalette::ButtonText, brush1);
        palette4.setBrush(QPalette::Active, QPalette::Base, brush);
        palette4.setBrush(QPalette::Active, QPalette::Window, brush);
        palette4.setBrush(QPalette::Active, QPalette::Shadow, brush8);
        palette4.setBrush(QPalette::Active, QPalette::Highlight, brush9);
        palette4.setBrush(QPalette::Active, QPalette::HighlightedText, brush10);
        palette4.setBrush(QPalette::Active, QPalette::Link, brush11);
        palette4.setBrush(QPalette::Active, QPalette::LinkVisited, brush12);
        palette4.setBrush(QPalette::Active, QPalette::AlternateBase, brush13);
        palette4.setBrush(QPalette::Active, QPalette::NoRole, brush14);
        palette4.setBrush(QPalette::Active, QPalette::ToolTipBase, brush15);
        palette4.setBrush(QPalette::Active, QPalette::ToolTipText, brush6);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette4.setBrush(QPalette::Active, QPalette::PlaceholderText, brush16);
#endif
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush6);
        palette4.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette4.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette4.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette4.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette4.setBrush(QPalette::Inactive, QPalette::Text, brush6);
        palette4.setBrush(QPalette::Inactive, QPalette::BrightText, brush7);
        palette4.setBrush(QPalette::Inactive, QPalette::ButtonText, brush6);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::Shadow, brush8);
        palette4.setBrush(QPalette::Inactive, QPalette::Highlight, brush9);
        palette4.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush10);
        palette4.setBrush(QPalette::Inactive, QPalette::Link, brush11);
        palette4.setBrush(QPalette::Inactive, QPalette::LinkVisited, brush12);
        palette4.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush13);
        QBrush brush23(QColor(0, 0, 0, 255));
        brush23.setStyle(Qt::NoBrush);
        palette4.setBrush(QPalette::Inactive, QPalette::NoRole, brush23);
        palette4.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush15);
        palette4.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush6);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette4.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush16);
#endif
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush18);
        palette4.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette4.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette4.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette4.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette4.setBrush(QPalette::Disabled, QPalette::Text, brush18);
        palette4.setBrush(QPalette::Disabled, QPalette::BrightText, brush7);
        palette4.setBrush(QPalette::Disabled, QPalette::ButtonText, brush18);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::Window, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::Shadow, brush19);
        palette4.setBrush(QPalette::Disabled, QPalette::Highlight, brush9);
        palette4.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush10);
        palette4.setBrush(QPalette::Disabled, QPalette::Link, brush11);
        palette4.setBrush(QPalette::Disabled, QPalette::LinkVisited, brush12);
        palette4.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush13);
        QBrush brush24(QColor(0, 0, 0, 255));
        brush24.setStyle(Qt::NoBrush);
        palette4.setBrush(QPalette::Disabled, QPalette::NoRole, brush24);
        palette4.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush15);
        palette4.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush6);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette4.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush16);
#endif
        push_button_3->setPalette(palette4);
        push_button_3->setFont(font1);

        verticalLayout->addWidget(push_button_3);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer_3);

        push_button_4 = new QPushButton(centralwidget);
        push_button_4->setObjectName(QString::fromUtf8("push_button_4"));
        push_button_4->setMinimumSize(QSize(419, 110));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush6);
        palette5.setBrush(QPalette::Active, QPalette::Button, brush);
        palette5.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette5.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        palette5.setBrush(QPalette::Active, QPalette::Dark, brush4);
        palette5.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette5.setBrush(QPalette::Active, QPalette::Text, brush6);
        palette5.setBrush(QPalette::Active, QPalette::BrightText, brush7);
        palette5.setBrush(QPalette::Active, QPalette::ButtonText, brush6);
        palette5.setBrush(QPalette::Active, QPalette::Base, brush);
        palette5.setBrush(QPalette::Active, QPalette::Window, brush);
        palette5.setBrush(QPalette::Active, QPalette::Shadow, brush8);
        palette5.setBrush(QPalette::Active, QPalette::Highlight, brush9);
        palette5.setBrush(QPalette::Active, QPalette::HighlightedText, brush10);
        palette5.setBrush(QPalette::Active, QPalette::Link, brush11);
        palette5.setBrush(QPalette::Active, QPalette::LinkVisited, brush12);
        palette5.setBrush(QPalette::Active, QPalette::AlternateBase, brush13);
        palette5.setBrush(QPalette::Active, QPalette::NoRole, brush14);
        palette5.setBrush(QPalette::Active, QPalette::ToolTipBase, brush15);
        palette5.setBrush(QPalette::Active, QPalette::ToolTipText, brush6);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette5.setBrush(QPalette::Active, QPalette::PlaceholderText, brush16);
#endif
        palette5.setBrush(QPalette::Inactive, QPalette::WindowText, brush6);
        palette5.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette5.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette5.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette5.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette5.setBrush(QPalette::Inactive, QPalette::Text, brush6);
        palette5.setBrush(QPalette::Inactive, QPalette::BrightText, brush7);
        palette5.setBrush(QPalette::Inactive, QPalette::ButtonText, brush6);
        palette5.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::Shadow, brush8);
        palette5.setBrush(QPalette::Inactive, QPalette::Highlight, brush9);
        palette5.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush10);
        palette5.setBrush(QPalette::Inactive, QPalette::Link, brush11);
        palette5.setBrush(QPalette::Inactive, QPalette::LinkVisited, brush12);
        palette5.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush13);
        QBrush brush25(QColor(0, 0, 0, 255));
        brush25.setStyle(Qt::NoBrush);
        palette5.setBrush(QPalette::Inactive, QPalette::NoRole, brush25);
        palette5.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush15);
        palette5.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush6);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette5.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush16);
#endif
        palette5.setBrush(QPalette::Disabled, QPalette::WindowText, brush18);
        palette5.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette5.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette5.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette5.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette5.setBrush(QPalette::Disabled, QPalette::Text, brush18);
        palette5.setBrush(QPalette::Disabled, QPalette::BrightText, brush7);
        palette5.setBrush(QPalette::Disabled, QPalette::ButtonText, brush18);
        palette5.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::Window, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::Shadow, brush19);
        palette5.setBrush(QPalette::Disabled, QPalette::Highlight, brush9);
        palette5.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush10);
        palette5.setBrush(QPalette::Disabled, QPalette::Link, brush11);
        palette5.setBrush(QPalette::Disabled, QPalette::LinkVisited, brush12);
        palette5.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush13);
        QBrush brush26(QColor(0, 0, 0, 255));
        brush26.setStyle(Qt::NoBrush);
        palette5.setBrush(QPalette::Disabled, QPalette::NoRole, brush26);
        palette5.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush15);
        palette5.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush6);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette5.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush16);
#endif
        push_button_4->setPalette(palette5);
        push_button_4->setFont(font1);

        verticalLayout->addWidget(push_button_4);


        verticalLayout_2->addLayout(verticalLayout);

        Main_Window->setCentralWidget(centralwidget);
        toolBar = new QToolBar(Main_Window);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        Main_Window->addToolBar(Qt::TopToolBarArea, toolBar);

        retranslateUi(Main_Window);

        QMetaObject::connectSlotsByName(Main_Window);
    } // setupUi

    void retranslateUi(QMainWindow *Main_Window)
    {
        Main_Window->setWindowTitle(QCoreApplication::translate("Main_Window", "Main_Window", nullptr));
        label->setText(QCoreApplication::translate("Main_Window", "Cybersecurity", nullptr));
        label_2->setText(QCoreApplication::translate("Main_Window", "Course", nullptr));
        push_button_1->setText(QCoreApplication::translate("Main_Window", "Rubber Ducky", nullptr));
        push_button_2->setText(QCoreApplication::translate("Main_Window", "Wi-Fi Jammer", nullptr));
        push_button_3->setText(QCoreApplication::translate("Main_Window", "Man-in-the-Middle", nullptr));
        push_button_4->setText(QCoreApplication::translate("Main_Window", "War Driving", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("Main_Window", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Main_Window: public Ui_Main_Window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_WINDOW_H
