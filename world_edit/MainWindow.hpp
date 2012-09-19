#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtSql>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtGui/QInputDialog>

//#include "shared/Templates.hpp"
#include "ui_MainWindow.h"

int InventoryType(int index);

enum InventoryWorldEditorType
{
    NotEquippable = 0,
    BagIndex = 1
};

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT
    enum ConnectionState
    {
        None,
        Server,
        Client
    };

public:
    explicit MainWindow(QWidget *parent = 0);

private slots:
    void on_invTypeBox_currentIndexChanged(const QString& newText);
    void on_tabWidgets_currentChanged(int index);

    void MakeTabNeedDatabaseConnectionState(bool State);
    void GenerateItemID();

    void on_actionExit_triggered();

    void on_actionA_client_database_triggered();

    void on_actionA_server_database_triggered();

    void on_actionSave_map_triggered();

    void on_actionA_map_triggered();

    void on_addItemToDatabase_clicked();

    void on_actionClient_database_triggered();

private:
    quint64 getGUID(bool* okay = nullptr);
    quint64 getNewItemID(bool* okay = nullptr);
    void MessageError(QString const& Message);

    void loadClientDatabase();
    void loadServerDatabase();

private:
    ConnectionState m_ConnectionState;

    QSqlDatabase    m_Server_DatabaseLink;
    QSqlQuery*      m_Server_Query;

    QString         m_Client_DatabaseFileName;
    QFile*          m_Client_DatabaseFile;

    /** QMap<quint64, ItemTemplate* > m_ItemCaches;
    QMap<quint64, CreatureTemplate* > m_CreatureCaches;
    QMap<quint64, QuestTemplate* > m_QuestCaches; **/

    int lastIndex;
};

#endif // MAINWINDOW_HPP
