#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), m_ConnectionState(None)
{
    setupUi(this);

    m_Server_Query = nullptr;
    m_Client_DatabaseFile = nullptr;
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
    quint64 uuid = getNewItemID(&connection);
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
quint64 MainWindow::getNewItemID(bool* connection)
{
    if (connection)
        *connection = true;

    if (m_ConnectionState & Server)
    {
        if (!m_Server_Query)
            m_Server_Query = new QSqlQuery(m_Server_DatabaseLink);

        if (connection)
        {
            *connection = false;
            return 0;
        }

        if (!m_Server_Query->exec("SELECT * FROM `item_template`;") && connection)
        {
            *connection = false;
            return 0;
        }

        m_Server_Query->first();
        return m_Server_Query->size() + 1;
    }
    else if (m_ConnectionState & Client)
        return QInputDialog::getInt(this, tr("World Editor"), tr("Enter an Item ID"));
    else
        return 0;
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

    m_Client_DatabaseFileName = QFileDialog::getOpenFileName(this, tr("World Editor"), QApplication::applicationDirPath(), tr("Data files (*.data);;Database files (*.db);;Database Client (*.dbc)"));
    m_Client_DatabaseFile = new QFile(m_Client_DatabaseFileName, this);

    if (m_Client_DatabaseFile->open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Append))
    {
        int reponse = QMessageBox::question(this, tr("World Editor"), tr("Client Database has been opened successfully.\nDo you want to cache data ?"), tr("Yes, please."), tr("No, it will be too long :P"));
        if (reponse == 0)
            loadClientDatabase();
    }
    else
    {
        m_ConnectionState = None;
        MakeTabNeedDatabaseConnectionState(true);
    }
}

void MainWindow::on_actionA_server_database_triggered()
{
    // If all is okay
    m_ConnectionState = Server;
    MakeTabNeedDatabaseConnectionState(false);

    QString hostname = QInputDialog::getText(this, tr("World Editor - Server Prompt"), tr("Host name ?"));
    QString username = QInputDialog::getText(this, tr("World Editor - Server Prompt"), tr("Username ?"));
    QString password = QInputDialog::getText(this, tr("World Editor - Server Prompt"), tr("Password ?"), QLineEdit::Password);
    QString world_database_name = QInputDialog::getText(this, tr("World Editor - Server Prompt"), tr("World Database Name ?"));
    qint32 port = QInputDialog::getInt(this, tr("World Editor - Server Prompt"), tr("Port ?"), 3306);

    m_Server_DatabaseLink = QSqlDatabase::addDatabase(tr("QMYSQL"), tr("World Editor - Connection MySQL"));
    m_Server_DatabaseLink.setHostName(hostname);
    m_Server_DatabaseLink.setDatabaseName(world_database_name);
    m_Server_DatabaseLink.setPort(port);

    if (m_Server_DatabaseLink.open(username, password))
    {
        int reponse = QMessageBox::question(this, tr("World Editor"), tr("Connection success !\nDo you want to cache data in memory ?"), tr("Yes, please."), tr("No, it will be too long :P"));
        if (reponse == 0)
        {
            loadServerDatabase();
        }
    }
    else
    {
        MessageError("Failed to connect to database...");
        m_ConnectionState = None;
        MakeTabNeedDatabaseConnectionState(true);
    }
}

void MainWindow::on_actionSave_map_triggered()
{
    // If all is okay
}

void MainWindow::on_actionA_map_triggered()
{

}

void MainWindow::loadClientDatabase()
{
    QTextStream in (m_Client_DatabaseFile);

    while (!in.atEnd())
    {
        QString line = in.readLine();

        QStringList data = line.split(';');

        if (data[0] == "CreatureTemplate")
        {
            // CreatureTemplate* tmpl = new CreatureTemplate(data[1].toULongLong(), data[2], data[3], data[4].toUInt(), data[5].toUInt(), data[6].toUInt(), data[7].toUInt(), data[8].toUInt(), data[9]);
        }
        else if (data[0] == "ItemTemplate")
        {
            // ItemTemplate* tmpl = new ItemTemplate(data[1].toULongLong(), data[2].toUInt(), data[3].toUInt(), data[4], data[5].toUInt(), data[6].toUInt(), data[7].toUInt(), data[8]);
        }
        else if (data[1] == "QuestTemplate")
        {
            // QuestTemplate* tmpl = new QuestTemplate(data[1].toULongLong(), data[2], data[3], data[4]);
        }
    }
}

void MainWindow::loadServerDatabase()
{
    if (!m_Server_Query)
        m_Server_Query = new QSqlQuery(m_Server_DatabaseLink);

    // TODO: Do it.
}

void MainWindow::on_addItemToDatabase_clicked()
{
    if (m_ConnectionState & Server)
    {
        QString query = "INSERT INTO `item_template` VALUES(:entry, :class, :subclass, :name, :displayid, :inventory_type, :container_slots, :description);";

        if (!m_Server_Query)
            m_Server_Query = new QSqlQuery(m_Server_DatabaseLink);

        if (!m_Server_Query->prepare(query))
        {
            MessageError(tr("Failed to prepare a query !\n") + m_Server_Query->lastError().text());
            return;
        }

        m_Server_Query->bindValue(":entry", QVariant(itemID_Line->text().toULongLong()));
        m_Server_Query->bindValue(":name", QVariant(nameEdit->text()));
        m_Server_Query->bindValue(":displayid", QVariant(displayIDEdit->text().toUInt()));
        m_Server_Query->bindValue(":inventory_type", QVariant(InventoryType(invTypeBox->currentIndex())));
        m_Server_Query->bindValue(":container_slots", invTypeBox->currentIndex() == BagIndex ? QVariant(containerSlotsBox->value()) : QVariant(0));
        m_Server_Query->bindValue(":description", QVariant(nameDescription->text()));
        m_Server_Query->bindValue(":class", QVariant(classBox->currentIndex()));
        m_Server_Query->bindValue(":subclass", QVariant(subClassBox->currentIndex()));

        if (!m_Server_Query->exec())
        {
            MessageError(tr("Failed to add item to database.\n") + m_Server_Query->lastError().text());
            return;
        }

        QMessageBox::information(this, tr("World Editor"), tr("%1 has been added in database").arg(nameEdit->text()));
    }
    else if (m_ConnectionState & Client)
    {
        QTextStream out(m_Client_DatabaseFile);

        // Header
        out << "ItemTemplate;";

        // Data
        out << itemID_Line->text().toULongLong() << ";";
        out << classBox->currentIndex() << ";";
        out << subClassBox->currentIndex() << ";";
        out << nameEdit->text() << ";";
        out << displayIDEdit->text().toUInt() << ";";
        out << invTypeBox->currentIndex() << ";";
        out << ((invTypeBox->currentIndex() == BagIndex) ? containerSlotsBox->value() : 0) << ";";
        out << nameDescription->text();

        // End
        out << '\n';
    }
    else
    {
        MessageError("No connection is available !");
    }
}

int InventoryType(int index)
{
    return index;
}

void MainWindow::on_actionClient_database_triggered()
{
    m_Client_DatabaseFileName = QFileDialog::getSaveFileName(this, tr("World Editor"), QApplication::applicationDirPath(), tr("Client database files (.dbc)"));
    m_Client_DatabaseFile = new QFile(m_Client_DatabaseFileName, this);

    if (m_Client_DatabaseFile->open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
    {
        QMessageBox::information(this, tr("World Editor"), tr("%1 has been opened successfully, you can edit database now.").arg(m_Client_DatabaseFileName));
        m_ConnectionState = Client;
        MakeTabNeedDatabaseConnectionState(false);
    }
    else
    {
        MessageError(tr("Unable to open %1").arg(m_Client_DatabaseFileName));
    }
}
