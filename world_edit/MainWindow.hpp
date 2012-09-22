#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtSql>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtGui/QInputDialog>

#include "Templates.hpp"
#include "ui_MainWindow.h"

int PInventoryType(int index);

enum InventoryWorldEditorType
{
    NotEquippable = 0,
    BagIndex = 1
};

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private slots:
    void on_invTypeBox_currentIndexChanged(const QString& newText);
    void on_tabWidgets_currentChanged(int index);

    void MakeTabNeedDatabaseConnectionState(bool State);

    void on_actionExit_triggered(); // Exit
    void on_actionA_server_database_triggered(); // Open server data connection

    void on_addItemToDatabase_clicked(); // Add Item to Database
    void on_addCreatureToDatabase_clicked(); // Add creature to Database

    void on_actionMerge_server_database_to_client_database_triggered();

private:
    quint64 getGUID(bool* okay = nullptr); // Get GUID
    quint64 getNewItemID(bool* okay = nullptr); // Get New Item ID
    quint64 getNewCreatureID(bool* okay = nullptr); // Get New Creature ID
    quint64 getNewQuestID(bool* okay = nullptr); // Get New Quest ID

    void MessageError(QString const& Message);

    void loadServerDatabase(); // Cache all data

    void GenerateItemID();
    void GenerateCreatureID();
    void GenerateQuestID();

    int determineNpcFlag();

private:
    bool IsConnected;
    QSqlDatabase    m_Server_DatabaseLink;
    QSqlQuery*      m_Server_Query;

    QMap<quint64, ItemTemplate* > m_ItemCaches;
    QMap<quint64, CreatureTemplate* > m_CreatureCaches;
    QMap<quint64, QuestTemplate* > m_QuestCaches;

    int lastIndex;
};

#endif // MAINWINDOW_HPP
