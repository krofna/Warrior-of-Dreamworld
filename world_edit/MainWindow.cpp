#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), m_ConnectionState(None)
{
    setupUi(this);

    m_Server_Query = NULL;
    lastIndex = 0;

    MakeTabNeedDatabaseConnectionState(true);
}

void MainWindow::on_tabWidgets_currentChanged(int index)
{
    if (index == 0)
    {

    }
    else if (index == 1)
    {
        if (m_ConnectionState == None)
        {
            MessageError("You must connect to a database system first !");
            tabWidgets->setCurrentIndex(lastIndex);
            return;
        }

        GenerateItemID();
    }
    else if (index == 2)
    {
        if (m_ConnectionState == None)
        {
            MessageError("You must connect to a database system first !");
            tabWidgets->setCurrentIndex(lastIndex);
            return;
        }
    }
    else if (index == 3)
    {

    }
    else
    {

    }
    lastIndex = index;
}

void MainWindow::on_invTypeBox_currentIndexChanged(const QString &newText)
{
    if (newText == "Bag")
        bag_specific_group->setDisabled(false);
    else
    {
        if (bag_specific_group->isEnabled())
            bag_specific_group->setDisabled(true);
    }
}

void MainWindow::MakeTabNeedDatabaseConnectionState(bool State)
{
    tabItem->setDisabled(State);
    tabDisplayID->setDisabled(State);
}


void MainWindow::GenerateItemID()
{
    bool connection;
    quint64 uuid = getGUID(&connection);
    if (!connection)
    {
        MessageError(tr("Failed to get a new Item ID, check connection with DB !"));
        return;
    }

    QString itemID_String = QString::number(uuid);

    itemID_Line->setText(itemID_String);
}

quint64 MainWindow::getGUID(bool* okay)
{
    if (okay)
        *okay = true;

    if (okay && ((m_ConnectionState & Server) == 0 || !m_Server_Query))
    {
        *okay = false;
        return 0;
    }

    if (!m_Server_Query->exec("SELECT UUID_SHORT();") && okay)
    {
        *okay = false;
        return 0;
    }

    m_Server_Query->first();
    return m_Server_Query->value(0).value<quint64>();
}

void MainWindow::MessageError(const QString &Message)
{
    QMessageBox::critical(this, "World Editor", Message);
}

void MainWindow::on_actionExit_triggered()
{
    qApp->exit();
}

void MainWindow::on_actionA_client_database_triggered()
{
    // If all is okay.
    m_ConnectionState = Client;
    MakeTabNeedDatabaseConnectionState(false);
}

void MainWindow::on_actionA_server_database_triggered()
{
    // If all is okay
    m_ConnectionState = Server;
    MakeTabNeedDatabaseConnectionState(false);
}

void MainWindow::on_actionSave_map_triggered()
{
    // If all is okay
}

void MainWindow::on_actionA_map_triggered()
{

}
