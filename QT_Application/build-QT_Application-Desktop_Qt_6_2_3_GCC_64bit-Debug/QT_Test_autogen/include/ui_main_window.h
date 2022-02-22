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
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Main_Window
{
public:
    QAction *action_home;
    QWidget *Central_Widget;
    QVBoxLayout *verticalLayout_2;
    QStackedWidget *Stacked_Widget;
    QWidget *Main_Menu_Widget;
    QVBoxLayout *verticalLayout;
    QSpacerItem *horizontalSpacer_6;
    QLabel *label;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_7;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *rubber_ducky_button;
    QSpacerItem *horizontalSpacer;
    QPushButton *wifi_jammer_button;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *man_in_the_middle_button;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *war_driving_button;
    QWidget *Rubber_Ducky_Widget;
    QPushButton *pushButton;
    QWidget *WiFi_Jammer_Widget;
    QPushButton *pushButton_2;
    QWidget *Man_in_the_Middle_Widget;
    QPushButton *pushButton_3;
    QWidget *War_Driving_Widget;
    QPushButton *pushButton_4;
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
        action_home = new QAction(Main_Window);
        action_home->setObjectName(QString::fromUtf8("action_home"));
        Central_Widget = new QWidget(Main_Window);
        Central_Widget->setObjectName(QString::fromUtf8("Central_Widget"));
        Central_Widget->setEnabled(true);
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
        Central_Widget->setPalette(palette1);
        Central_Widget->setStyleSheet(QString::fromUtf8("background-color: rgb(30, 34, 39);"));
        verticalLayout_2 = new QVBoxLayout(Central_Widget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        Stacked_Widget = new QStackedWidget(Central_Widget);
        Stacked_Widget->setObjectName(QString::fromUtf8("Stacked_Widget"));
        Main_Menu_Widget = new QWidget();
        Main_Menu_Widget->setObjectName(QString::fromUtf8("Main_Menu_Widget"));
        verticalLayout = new QVBoxLayout(Main_Menu_Widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(20, -1, 20, 30);
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer_6);

        label = new QLabel(Main_Menu_Widget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setPointSize(36);
        font.setBold(true);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("color: rgb(218, 218, 218);"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        label_2 = new QLabel(Main_Menu_Widget);
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

        rubber_ducky_button = new QPushButton(Main_Menu_Widget);
        rubber_ducky_button->setObjectName(QString::fromUtf8("rubber_ducky_button"));
        rubber_ducky_button->setEnabled(true);
        rubber_ducky_button->setMinimumSize(QSize(419, 110));
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
        rubber_ducky_button->setPalette(palette2);
        QFont font1;
        font1.setPointSize(26);
        font1.setBold(true);
        rubber_ducky_button->setFont(font1);

        verticalLayout->addWidget(rubber_ducky_button);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer);

        wifi_jammer_button = new QPushButton(Main_Menu_Widget);
        wifi_jammer_button->setObjectName(QString::fromUtf8("wifi_jammer_button"));
        wifi_jammer_button->setMinimumSize(QSize(419, 110));
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
        wifi_jammer_button->setPalette(palette3);
        wifi_jammer_button->setFont(font1);

        verticalLayout->addWidget(wifi_jammer_button);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer_2);

        man_in_the_middle_button = new QPushButton(Main_Menu_Widget);
        man_in_the_middle_button->setObjectName(QString::fromUtf8("man_in_the_middle_button"));
        man_in_the_middle_button->setMinimumSize(QSize(419, 110));
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
        man_in_the_middle_button->setPalette(palette4);
        man_in_the_middle_button->setFont(font1);

        verticalLayout->addWidget(man_in_the_middle_button);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer_3);

        war_driving_button = new QPushButton(Main_Menu_Widget);
        war_driving_button->setObjectName(QString::fromUtf8("war_driving_button"));
        war_driving_button->setMinimumSize(QSize(419, 110));
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
        war_driving_button->setPalette(palette5);
        war_driving_button->setFont(font1);

        verticalLayout->addWidget(war_driving_button);

        Stacked_Widget->addWidget(Main_Menu_Widget);
        Rubber_Ducky_Widget = new QWidget();
        Rubber_Ducky_Widget->setObjectName(QString::fromUtf8("Rubber_Ducky_Widget"));
        pushButton = new QPushButton(Rubber_Ducky_Widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(90, 280, 291, 81));
        QFont font2;
        font2.setPointSize(28);
        pushButton->setFont(font2);
        Stacked_Widget->addWidget(Rubber_Ducky_Widget);
        WiFi_Jammer_Widget = new QWidget();
        WiFi_Jammer_Widget->setObjectName(QString::fromUtf8("WiFi_Jammer_Widget"));
        pushButton_2 = new QPushButton(WiFi_Jammer_Widget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(80, 310, 291, 81));
        pushButton_2->setFont(font2);
        Stacked_Widget->addWidget(WiFi_Jammer_Widget);
        Man_in_the_Middle_Widget = new QWidget();
        Man_in_the_Middle_Widget->setObjectName(QString::fromUtf8("Man_in_the_Middle_Widget"));
        pushButton_3 = new QPushButton(Man_in_the_Middle_Widget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(50, 300, 351, 81));
        pushButton_3->setFont(font2);
        Stacked_Widget->addWidget(Man_in_the_Middle_Widget);
        War_Driving_Widget = new QWidget();
        War_Driving_Widget->setObjectName(QString::fromUtf8("War_Driving_Widget"));
        pushButton_4 = new QPushButton(War_Driving_Widget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(70, 270, 321, 81));
        pushButton_4->setFont(font2);
        Stacked_Widget->addWidget(War_Driving_Widget);

        verticalLayout_2->addWidget(Stacked_Widget);

        Main_Window->setCentralWidget(Central_Widget);
        toolBar = new QToolBar(Main_Window);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setMinimumSize(QSize(480, 50));
        Main_Window->addToolBar(Qt::TopToolBarArea, toolBar);

        toolBar->addAction(action_home);

        retranslateUi(Main_Window);

        Stacked_Widget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Main_Window);
    } // setupUi

    void retranslateUi(QMainWindow *Main_Window)
    {
        Main_Window->setWindowTitle(QCoreApplication::translate("Main_Window", "Main_Window", nullptr));
        action_home->setText(QCoreApplication::translate("Main_Window", "Home", nullptr));
        label->setText(QCoreApplication::translate("Main_Window", "Cybersecurity", nullptr));
        label_2->setText(QCoreApplication::translate("Main_Window", "Course", nullptr));
        rubber_ducky_button->setText(QCoreApplication::translate("Main_Window", "Rubber Ducky", nullptr));
        wifi_jammer_button->setText(QCoreApplication::translate("Main_Window", "Wi-Fi Jammer", nullptr));
        man_in_the_middle_button->setText(QCoreApplication::translate("Main_Window", "Man-in-the-Middle", nullptr));
        war_driving_button->setText(QCoreApplication::translate("Main_Window", "War Driving", nullptr));
        pushButton->setText(QCoreApplication::translate("Main_Window", "Rubber Duck", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Main_Window", "WiFi Jammer", nullptr));
        pushButton_3->setText(QCoreApplication::translate("Main_Window", "Man in the Middle", nullptr));
        pushButton_4->setText(QCoreApplication::translate("Main_Window", "War Driving", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("Main_Window", "Tool_Bar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Main_Window: public Ui_Main_Window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_WINDOW_H
