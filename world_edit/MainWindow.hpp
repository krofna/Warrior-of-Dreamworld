#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtSql>
#include <QtGui/QMessageBox>
#include "ui_MainWindow.h"

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

private:
    quint64 getGUID(bool* okay = NULL);
    void MessageError(QString const& Message);

private:
    ConnectionState m_ConnectionState;

    QSqlDatabase    m_Server_DatabaseLink;
    QSqlQuery*      m_Server_Query;

    QString         m_Client_DatabaseFile;

    int lastIndex;
};

#endif // MAINWINDOW_HPP
