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
    QAction *action_help;
    QWidget *Central_Widget;
    QVBoxLayout *verticalLayout_2;
    QStackedWidget *Stacked_Widget;
    QWidget *Main_Menu_Widget;
    QVBoxLayout *verticalLayout;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *horizontalSpacer_7;
    QLabel *label;
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
    QToolBar *Tool_Bar;

    void setupUi(QMainWindow *Main_Window)
    {
        if (Main_Window->objectName().isEmpty())
            Main_Window->setObjectName(QString::fromUtf8("Main_Window"));
        Main_Window->resize(480, 800);
        Main_Window->setMinimumSize(QSize(480, 800));
        Main_Window->setMaximumSize(QSize(480, 800));
        Main_Window->setStyleSheet(QString::fromUtf8("background-color: rgb(40, 44, 52);"));
        action_home = new QAction(Main_Window);
        action_home->setObjectName(QString::fromUtf8("action_home"));
        QIcon icon;
        QString iconThemeName = QString::fromUtf8("user-home-symbolic");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        action_home->setIcon(icon);
        action_help = new QAction(Main_Window);
        action_help->setObjectName(QString::fromUtf8("action_help"));
        QIcon icon1;
        iconThemeName = QString::fromUtf8("help-hint");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon1 = QIcon::fromTheme(iconThemeName);
        } else {
            icon1.addFile(QString::fromUtf8("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        action_help->setIcon(icon1);
        Central_Widget = new QWidget(Main_Window);
        Central_Widget->setObjectName(QString::fromUtf8("Central_Widget"));
        Central_Widget->setEnabled(true);
        Central_Widget->setStyleSheet(QString::fromUtf8("background-color: rgb(30, 34, 39);\n"
""));
        verticalLayout_2 = new QVBoxLayout(Central_Widget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        Stacked_Widget = new QStackedWidget(Central_Widget);
        Stacked_Widget->setObjectName(QString::fromUtf8("Stacked_Widget"));
        Stacked_Widget->setEnabled(true);
        Main_Menu_Widget = new QWidget();
        Main_Menu_Widget->setObjectName(QString::fromUtf8("Main_Menu_Widget"));
        verticalLayout = new QVBoxLayout(Main_Menu_Widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(20, -1, 20, 30);
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer_6);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer_7);

        label = new QLabel(Main_Menu_Widget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setFamilies({QString::fromUtf8("DejaVu Serif")});
        font.setPointSize(45);
        font.setBold(true);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("color: rgb(220, 225, 232);"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer_4);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer_5);

        rubber_ducky_button = new QPushButton(Main_Menu_Widget);
        rubber_ducky_button->setObjectName(QString::fromUtf8("rubber_ducky_button"));
        rubber_ducky_button->setMinimumSize(QSize(419, 100));
        QFont font1;
        font1.setPointSize(26);
        font1.setBold(true);
        rubber_ducky_button->setFont(font1);
        rubber_ducky_button->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	color: rgb(209, 213, 218);\n"
"	background-color: rgb(40, 44, 52);\n"
"}"));

        verticalLayout->addWidget(rubber_ducky_button);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer);

        wifi_jammer_button = new QPushButton(Main_Menu_Widget);
        wifi_jammer_button->setObjectName(QString::fromUtf8("wifi_jammer_button"));
        wifi_jammer_button->setMinimumSize(QSize(419, 100));
        wifi_jammer_button->setFont(font1);
        wifi_jammer_button->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	color: rgb(209, 213, 218);\n"
"	background-color: rgb(40, 44, 52);\n"
"}"));

        verticalLayout->addWidget(wifi_jammer_button);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer_2);

        man_in_the_middle_button = new QPushButton(Main_Menu_Widget);
        man_in_the_middle_button->setObjectName(QString::fromUtf8("man_in_the_middle_button"));
        man_in_the_middle_button->setMinimumSize(QSize(419, 100));
        man_in_the_middle_button->setFont(font1);
        man_in_the_middle_button->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	color: rgb(209, 213, 218);\n"
"	background-color: rgb(40, 44, 52);\n"
"}"));

        verticalLayout->addWidget(man_in_the_middle_button);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        verticalLayout->addItem(horizontalSpacer_3);

        war_driving_button = new QPushButton(Main_Menu_Widget);
        war_driving_button->setObjectName(QString::fromUtf8("war_driving_button"));
        war_driving_button->setMinimumSize(QSize(419, 100));
        war_driving_button->setFont(font1);
        war_driving_button->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	color: rgb(209, 213, 218);\n"
"	background-color: rgb(40, 44, 52);\n"
"}"));

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
        pushButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: rgb(40, 44, 52);\n"
"}"));
        Stacked_Widget->addWidget(Rubber_Ducky_Widget);
        WiFi_Jammer_Widget = new QWidget();
        WiFi_Jammer_Widget->setObjectName(QString::fromUtf8("WiFi_Jammer_Widget"));
        pushButton_2 = new QPushButton(WiFi_Jammer_Widget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(80, 310, 291, 81));
        pushButton_2->setFont(font2);
        pushButton_2->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: rgb(40, 44, 52);\n"
"}"));
        Stacked_Widget->addWidget(WiFi_Jammer_Widget);
        Man_in_the_Middle_Widget = new QWidget();
        Man_in_the_Middle_Widget->setObjectName(QString::fromUtf8("Man_in_the_Middle_Widget"));
        pushButton_3 = new QPushButton(Man_in_the_Middle_Widget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(50, 300, 351, 81));
        pushButton_3->setFont(font2);
        pushButton_3->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: rgb(40, 44, 52);\n"
"}"));
        Stacked_Widget->addWidget(Man_in_the_Middle_Widget);
        War_Driving_Widget = new QWidget();
        War_Driving_Widget->setObjectName(QString::fromUtf8("War_Driving_Widget"));
        pushButton_4 = new QPushButton(War_Driving_Widget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(70, 270, 321, 81));
        pushButton_4->setFont(font2);
        pushButton_4->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	background-color: rgb(40, 44, 52);\n"
"}"));
        Stacked_Widget->addWidget(War_Driving_Widget);

        verticalLayout_2->addWidget(Stacked_Widget);

        Main_Window->setCentralWidget(Central_Widget);
        Tool_Bar = new QToolBar(Main_Window);
        Tool_Bar->setObjectName(QString::fromUtf8("Tool_Bar"));
        Tool_Bar->setLayoutDirection(Qt::RightToLeft);
        Tool_Bar->setStyleSheet(QString::fromUtf8("QToolButton {\n"
"	margin: 0px 10px;\n"
"}"));
        Tool_Bar->setMovable(false);
        Tool_Bar->setIconSize(QSize(34, 34));
        Tool_Bar->setFloatable(false);
        Main_Window->addToolBar(Qt::BottomToolBarArea, Tool_Bar);

        Tool_Bar->addAction(action_home);
        Tool_Bar->addAction(action_help);

        retranslateUi(Main_Window);

        Stacked_Widget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Main_Window);
    } // setupUi

    void retranslateUi(QMainWindow *Main_Window)
    {
        Main_Window->setWindowTitle(QCoreApplication::translate("Main_Window", "Main_Window", nullptr));
        action_home->setText(QCoreApplication::translate("Main_Window", "Home", nullptr));
        action_help->setText(QCoreApplication::translate("Main_Window", "Help_Documentation", nullptr));
        label->setText(QCoreApplication::translate("Main_Window", "Cybercraft", nullptr));
        rubber_ducky_button->setText(QCoreApplication::translate("Main_Window", "Rubber Ducky", nullptr));
        wifi_jammer_button->setText(QCoreApplication::translate("Main_Window", "Wi-Fi Jammer", nullptr));
        man_in_the_middle_button->setText(QCoreApplication::translate("Main_Window", "Man-in-the-Middle", nullptr));
        war_driving_button->setText(QCoreApplication::translate("Main_Window", "War Driving", nullptr));
        pushButton->setText(QCoreApplication::translate("Main_Window", "Rubber Duck", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Main_Window", "WiFi Jammer", nullptr));
        pushButton_3->setText(QCoreApplication::translate("Main_Window", "Man in the Middle", nullptr));
        pushButton_4->setText(QCoreApplication::translate("Main_Window", "War Driving", nullptr));
        Tool_Bar->setWindowTitle(QCoreApplication::translate("Main_Window", "Tool_Bar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Main_Window: public Ui_Main_Window {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_WINDOW_H
