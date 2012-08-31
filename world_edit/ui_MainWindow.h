/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created: Sat Sep 1 00:44:16 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFormLayout>
#include <QtGui/QGraphicsView>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen_a_connection_in_server_database;
    QAction *actionA_client_database;
    QAction *actionA_server_database;
    QAction *actionExit;
    QAction *actionA_map;
    QAction *actionMap;
    QAction *actionItem;
    QAction *actionDisplay_ID;
    QAction *actionSave_map;
    QWidget *centralWidget;
    QGridLayout *gridLayout_3;
    QTabWidget *tabWidgets;
    QWidget *tabMap;
    QVBoxLayout *verticalLayout;
    QGraphicsView *mapView;
    QWidget *tabItem;
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_4;
    QComboBox *subClassBox;
    QLabel *label_3;
    QComboBox *classBox;
    QLabel *label_7;
    QLabel *label_6;
    QSpinBox *displayIDEdit;
    QLineEdit *nameEdit;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *nameDescription;
    QLabel *label_2;
    QComboBox *invTypeBox;
    QSpacerItem *verticalSpacer_2;
    QLabel *label_8;
    QLineEdit *itemID_Line;
    QGroupBox *bag_specific_group;
    QGridLayout *gridLayout;
    QLabel *label;
    QSpinBox *containerSlotsBox;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;
    QPushButton *addItemToDatabase;
    QWidget *tabDisplayID;
    QVBoxLayout *verticalLayout_2;
    QFormLayout *formLayout;
    QLabel *displayInfoIDLabel;
    QSpinBox *displayInfoID_SpinBox;
    QLabel *textureFilenameLabel;
    QLineEdit *textureFilenameEdit;
    QSpacerItem *verticalSpacer_3;
    QPushButton *addDisplayIDToDatabase;
    QWidget *tabHelp;
    QVBoxLayout *verticalLayout_3;
    QPlainTextEdit *helpEdit;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuOpen;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(678, 502);
        actionOpen_a_connection_in_server_database = new QAction(MainWindow);
        actionOpen_a_connection_in_server_database->setObjectName(QString::fromUtf8("actionOpen_a_connection_in_server_database"));
        actionA_client_database = new QAction(MainWindow);
        actionA_client_database->setObjectName(QString::fromUtf8("actionA_client_database"));
        actionA_server_database = new QAction(MainWindow);
        actionA_server_database->setObjectName(QString::fromUtf8("actionA_server_database"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionA_map = new QAction(MainWindow);
        actionA_map->setObjectName(QString::fromUtf8("actionA_map"));
        actionMap = new QAction(MainWindow);
        actionMap->setObjectName(QString::fromUtf8("actionMap"));
        actionItem = new QAction(MainWindow);
        actionItem->setObjectName(QString::fromUtf8("actionItem"));
        actionDisplay_ID = new QAction(MainWindow);
        actionDisplay_ID->setObjectName(QString::fromUtf8("actionDisplay_ID"));
        actionSave_map = new QAction(MainWindow);
        actionSave_map->setObjectName(QString::fromUtf8("actionSave_map"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_3 = new QGridLayout(centralWidget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        tabWidgets = new QTabWidget(centralWidget);
        tabWidgets->setObjectName(QString::fromUtf8("tabWidgets"));
        tabMap = new QWidget();
        tabMap->setObjectName(QString::fromUtf8("tabMap"));
        verticalLayout = new QVBoxLayout(tabMap);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        mapView = new QGraphicsView(tabMap);
        mapView->setObjectName(QString::fromUtf8("mapView"));

        verticalLayout->addWidget(mapView);

        tabWidgets->addTab(tabMap, QString());
        tabItem = new QWidget();
        tabItem->setObjectName(QString::fromUtf8("tabItem"));
        gridLayout_2 = new QGridLayout(tabItem);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        groupBox = new QGroupBox(tabItem);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_4 = new QGridLayout(groupBox);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        subClassBox = new QComboBox(groupBox);
        subClassBox->setObjectName(QString::fromUtf8("subClassBox"));

        gridLayout_4->addWidget(subClassBox, 5, 1, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_4->addWidget(label_3, 3, 0, 1, 1);

        classBox = new QComboBox(groupBox);
        classBox->setObjectName(QString::fromUtf8("classBox"));

        gridLayout_4->addWidget(classBox, 4, 1, 1, 1);

        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_4->addWidget(label_7, 5, 0, 1, 1);

        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_4->addWidget(label_6, 4, 0, 1, 1);

        displayIDEdit = new QSpinBox(groupBox);
        displayIDEdit->setObjectName(QString::fromUtf8("displayIDEdit"));

        gridLayout_4->addWidget(displayIDEdit, 7, 1, 1, 1);

        nameEdit = new QLineEdit(groupBox);
        nameEdit->setObjectName(QString::fromUtf8("nameEdit"));

        gridLayout_4->addWidget(nameEdit, 1, 1, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_4->addWidget(label_4, 6, 0, 1, 1);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_4->addWidget(label_5, 7, 0, 1, 1);

        nameDescription = new QLineEdit(groupBox);
        nameDescription->setObjectName(QString::fromUtf8("nameDescription"));

        gridLayout_4->addWidget(nameDescription, 3, 1, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_4->addWidget(label_2, 1, 0, 1, 1);

        invTypeBox = new QComboBox(groupBox);
        invTypeBox->setObjectName(QString::fromUtf8("invTypeBox"));

        gridLayout_4->addWidget(invTypeBox, 6, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_2, 8, 1, 1, 1);

        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_4->addWidget(label_8, 0, 0, 1, 1);

        itemID_Line = new QLineEdit(groupBox);
        itemID_Line->setObjectName(QString::fromUtf8("itemID_Line"));
        itemID_Line->setReadOnly(true);

        gridLayout_4->addWidget(itemID_Line, 0, 1, 1, 1);


        gridLayout_2->addWidget(groupBox, 1, 0, 1, 1);

        bag_specific_group = new QGroupBox(tabItem);
        bag_specific_group->setObjectName(QString::fromUtf8("bag_specific_group"));
        bag_specific_group->setEnabled(false);
        gridLayout = new QGridLayout(bag_specific_group);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(bag_specific_group);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        containerSlotsBox = new QSpinBox(bag_specific_group);
        containerSlotsBox->setObjectName(QString::fromUtf8("containerSlotsBox"));
        containerSlotsBox->setMinimum(1);
        containerSlotsBox->setMaximum(36);

        gridLayout->addWidget(containerSlotsBox, 0, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 1, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);


        gridLayout_2->addWidget(bag_specific_group, 1, 1, 1, 1);

        addItemToDatabase = new QPushButton(tabItem);
        addItemToDatabase->setObjectName(QString::fromUtf8("addItemToDatabase"));

        gridLayout_2->addWidget(addItemToDatabase, 2, 0, 1, 2);

        tabWidgets->addTab(tabItem, QString());
        tabDisplayID = new QWidget();
        tabDisplayID->setObjectName(QString::fromUtf8("tabDisplayID"));
        verticalLayout_2 = new QVBoxLayout(tabDisplayID);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        formLayout = new QFormLayout();
        formLayout->setSpacing(6);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        displayInfoIDLabel = new QLabel(tabDisplayID);
        displayInfoIDLabel->setObjectName(QString::fromUtf8("displayInfoIDLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, displayInfoIDLabel);

        displayInfoID_SpinBox = new QSpinBox(tabDisplayID);
        displayInfoID_SpinBox->setObjectName(QString::fromUtf8("displayInfoID_SpinBox"));

        formLayout->setWidget(0, QFormLayout::FieldRole, displayInfoID_SpinBox);

        textureFilenameLabel = new QLabel(tabDisplayID);
        textureFilenameLabel->setObjectName(QString::fromUtf8("textureFilenameLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, textureFilenameLabel);

        textureFilenameEdit = new QLineEdit(tabDisplayID);
        textureFilenameEdit->setObjectName(QString::fromUtf8("textureFilenameEdit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, textureFilenameEdit);


        verticalLayout_2->addLayout(formLayout);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_3);

        addDisplayIDToDatabase = new QPushButton(tabDisplayID);
        addDisplayIDToDatabase->setObjectName(QString::fromUtf8("addDisplayIDToDatabase"));

        verticalLayout_2->addWidget(addDisplayIDToDatabase);

        tabWidgets->addTab(tabDisplayID, QString());
        tabHelp = new QWidget();
        tabHelp->setObjectName(QString::fromUtf8("tabHelp"));
        verticalLayout_3 = new QVBoxLayout(tabHelp);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        helpEdit = new QPlainTextEdit(tabHelp);
        helpEdit->setObjectName(QString::fromUtf8("helpEdit"));
        helpEdit->setReadOnly(true);

        verticalLayout_3->addWidget(helpEdit);

        tabWidgets->addTab(tabHelp, QString());

        gridLayout_3->addWidget(tabWidgets, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 678, 20));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuOpen = new QMenu(menuFile);
        menuOpen->setObjectName(QString::fromUtf8("menuOpen"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(menuOpen->menuAction());
        menuFile->addAction(actionSave_map);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuOpen->addAction(actionA_client_database);
        menuOpen->addAction(actionA_server_database);
        menuOpen->addAction(actionA_map);

        retranslateUi(MainWindow);

        tabWidgets->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionOpen_a_connection_in_server_database->setText(QApplication::translate("MainWindow", "Open a connection in server database", 0, QApplication::UnicodeUTF8));
        actionA_client_database->setText(QApplication::translate("MainWindow", "a client database", 0, QApplication::UnicodeUTF8));
        actionA_server_database->setText(QApplication::translate("MainWindow", "a server database", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        actionA_map->setText(QApplication::translate("MainWindow", "a map", 0, QApplication::UnicodeUTF8));
        actionMap->setText(QApplication::translate("MainWindow", "map", 0, QApplication::UnicodeUTF8));
        actionItem->setText(QApplication::translate("MainWindow", "item", 0, QApplication::UnicodeUTF8));
        actionDisplay_ID->setText(QApplication::translate("MainWindow", "display ID", 0, QApplication::UnicodeUTF8));
        actionSave_map->setText(QApplication::translate("MainWindow", "Save map", 0, QApplication::UnicodeUTF8));
        tabWidgets->setTabText(tabWidgets->indexOf(tabMap), QApplication::translate("MainWindow", "Map", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "Generic", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Description", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "Sub class", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "Class", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Inventory Type", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "Display ID", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Name", 0, QApplication::UnicodeUTF8));
        invTypeBox->clear();
        invTypeBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Not equippable", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Bag", 0, QApplication::UnicodeUTF8)
        );
        label_8->setText(QApplication::translate("MainWindow", "ItemID", 0, QApplication::UnicodeUTF8));
        bag_specific_group->setTitle(QApplication::translate("MainWindow", "Bag-specific", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Container slots:", 0, QApplication::UnicodeUTF8));
        addItemToDatabase->setText(QApplication::translate("MainWindow", "Add to database", 0, QApplication::UnicodeUTF8));
        tabWidgets->setTabText(tabWidgets->indexOf(tabItem), QApplication::translate("MainWindow", "Item", 0, QApplication::UnicodeUTF8));
        displayInfoIDLabel->setText(QApplication::translate("MainWindow", "DisplayInfoID", 0, QApplication::UnicodeUTF8));
        textureFilenameLabel->setText(QApplication::translate("MainWindow", "Texture Filename", 0, QApplication::UnicodeUTF8));
        addDisplayIDToDatabase->setText(QApplication::translate("MainWindow", "Add to database", 0, QApplication::UnicodeUTF8));
        tabWidgets->setTabText(tabWidgets->indexOf(tabDisplayID), QApplication::translate("MainWindow", "DisplayIDs", 0, QApplication::UnicodeUTF8));
        helpEdit->setPlainText(QApplication::translate("MainWindow", "Here is the help :D\n"
"But, I won't help you :)", 0, QApplication::UnicodeUTF8));
        tabWidgets->setTabText(tabWidgets->indexOf(tabHelp), QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuOpen->setTitle(QApplication::translate("MainWindow", "Open ...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
