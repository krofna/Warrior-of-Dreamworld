#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    m_Server_Query = nullptr;
    lastIndex = 0;
    IsConnected = false;

    MakeTabNeedDatabaseConnectionState(true);
}

void MainWindow::on_tabWidgets_currentChanged(int index)
{
    if (index == 0)
    {

    }
    else if (index == 1) // Creature
    {
        if (!IsConnected)
        {
            MessageError("You must connect to a database system first !");
            tabWidgets->setCurrentIndex(lastIndex);
            return;
        }

        GenerateCreatureID();
    }
    else if (index == 2) // Item
    {
        if (!IsConnected)
        {
            MessageError("You must connect to a database system first !");
            tabWidgets->setCurrentIndex(lastIndex);
            return;
        }

        GenerateItemID();
    }
    else if (index == 3) // Quest
    {
        if (!IsConnected)
        {
            MessageError("You must connect to a database system first !");
            tabWidgets->setCurrentIndex(lastIndex);
            return;
        }

        GenerateQuestID();
    }
    else if (index == 4) // Display ID
    {
        if (!IsConnected)
        {
            MessageError("You must connect to a database system first !");
            tabWidgets->setCurrentIndex(lastIndex);
            return;
        }
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
    tabCreature->setDisabled(State);
    tabQuest->setDisabled(State);
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

void MainWindow::GenerateCreatureID()
{
    bool connection;
    quint64 uuid = getNewCreatureID(&connection);
    if (!connection)
    {
        MessageError(tr("Failed to get a new Creature ID, check connection with DB !"));
        return;
    }

    QString creatureID_String = QString::number(uuid);
    creatureEntryLine->setText(creatureID_String);
}

void MainWindow::GenerateQuestID()
{
    bool connection;
    quint64 uuid = getNewQuestID(&connection);
    if (!connection)
    {
        MessageError(tr("Failed to get a new Quest ID, check connection with DB !"));
        return;
    }

    QString questID_String = QString::number(uuid);
    questIDLineEdit->setText(questID_String);
}

quint64 MainWindow::getGUID(bool* okay)
{
    if (okay)
        *okay = true;

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

    if (!m_Server_Query->exec("SELECT * FROM `item_template`;"))
    {
        if (connection)
            *connection = false;
        return 0;
    }

    m_Server_Query->first();
    return m_Server_Query->size() + 1;
}
quint64 MainWindow::getNewCreatureID(bool* connection)
{
    if (connection)
        *connection = true;

    if (!m_Server_Query->exec("SELECT * FROM `creature_template`;"))
    {
        if (connection)
            *connection = false;
        return 0;
    }

    m_Server_Query->first();
    return m_Server_Query->size() + 1;
}

quint64 MainWindow::getNewQuestID(bool* connection)
{
    if (connection)
        *connection = true;

    if (!m_Server_Query->exec("SELECT * FROM `quest_template`;"))
    {
        if (connection)
            *connection = false;
        return 0;
    }

    m_Server_Query->first();
    return m_Server_Query->size() + 1;
}

void MainWindow::MessageError(const QString &Message)
{
    QMessageBox::critical(this, "World Editor", Message);
}

void MainWindow::on_actionExit_triggered()
{
    qApp->exit();
}

void MainWindow::on_actionA_server_database_triggered()
{
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
        m_Server_Query = new QSqlQuery(m_Server_DatabaseLink);
        IsConnected = true;
        int answer = QMessageBox::question(this, tr("World Editor"), tr("Connection success !\nDo you want to cache data in memory ?"), tr("Yes, please."), tr("No, it will be too long :P"));
        if (answer == 0)
        {
            loadServerDatabase();
        }
    }
    else
    {
        MessageError("Failed to connect to database...");
        MakeTabNeedDatabaseConnectionState(true);
    }
}

void MainWindow::loadServerDatabase()
{
    // TODO: Do it.
}

void MainWindow::on_addItemToDatabase_clicked()
{
    QString query = "INSERT INTO `item_template` VALUES(:entry, :class, :subclass, :name, :displayid, :inventory_type, :container_slots, :description);";

    if (!m_Server_Query->prepare(query))
    {
        MessageError(tr("Failed to prepare a query !\n") + m_Server_Query->lastError().text());
        return;
    }

    m_Server_Query->bindValue(":entry", QVariant(itemID_Line->text().toULongLong()));
    m_Server_Query->bindValue(":name", QVariant(nameEdit->text()));
    m_Server_Query->bindValue(":displayid", QVariant(displayIDEdit->text().toUInt()));
    m_Server_Query->bindValue(":inventory_type", QVariant(PInventoryType(invTypeBox->currentIndex())));
    m_Server_Query->bindValue(":container_slots", invTypeBox->currentIndex() == BagIndex ? QVariant(containerSlotsBox->value()) : QVariant(0));
    m_Server_Query->bindValue(":description", QVariant(nameDescription->text()));
    m_Server_Query->bindValue(":class", QVariant(classBox->currentIndex()));
    m_Server_Query->bindValue(":subclass", QVariant(subClassBox->currentIndex()));

    if (!m_Server_Query->exec())
    {
        MessageError(tr("Failed to add item in database.\n") + m_Server_Query->lastError().text());
        return;
    }

    QMessageBox::information(this, tr("World Editor"), tr("%1 has been added in database").arg(nameEdit->text()));
}


void MainWindow::on_addCreatureToDatabase_clicked()
{
    QString query = "INSERT INTO `creature_template` VALUES(:id, :name, :tileset, :tx, :ty, :npcflag, :maxhealth, :maxpower, :scriptname);";

    if (!m_Server_Query->prepare(query))
    {
        MessageError(tr("Failed to prepare a query !\n") + m_Server_Query->lastError().text());
        return;
    }


    m_Server_Query->bindValue(":id", QVariant(creatureEntryLine->text().toULongLong()));
    m_Server_Query->bindValue(":name", QVariant(creatureNameLine->text()));
    m_Server_Query->bindValue(":tileset", QVariant(tilesetLineEdit->text()));
    m_Server_Query->bindValue(":tx", QVariant(tilesetXSpinBox->value()));
    m_Server_Query->bindValue(":ty", QVariant(tilesetYSpinBox->value()));
    m_Server_Query->bindValue(":npcflag", QVariant(determineNpcFlag()));
    m_Server_Query->bindValue(":maxhealth", QVariant(maxHealthSpinBox->value()));
    m_Server_Query->bindValue(":maxpower", QVariant(maxPowerSpinBox->value()));
    m_Server_Query->bindValue(":scriptname", QVariant(scriptNameLineEdit->text()));

    if (!m_Server_Query->exec())
    {
        MessageError(tr("Failed to add creature in database.\n") + m_Server_Query->lastError().text());
        return;
    }
    QMessageBox::information(this, tr("World Editor"), tr("%1 has been added in database").arg(creatureNameLine->text()));
}

int MainWindow::determineNpcFlag()
{
    int flag = 0;

    if (isQuestGiverCheck->isChecked())
        flag &= NPC_QUEST_GIVER;
    if (isVendorCheck->isChecked())
        flag &= NPC_VENDOR;
    if (isRepairCheck->isChecked())
        flag &= NPC_REPAIR;

    return flag;
}

int PInventoryType(int index)
{
    switch (index)
    {
    case 0:
        return INVTYPE_NON_EQUIP;
    case 1:
        return INVTYPE_BAG;
    default:
        return INVTYPE_NON_EQUIP;
    }
}

void MainWindow::on_actionMerge_server_database_to_client_database_triggered()
{
    QString hostname = QInputDialog::getText(this, tr("World Editor - Server Prompt"), tr("Host name ?"));
    QString username = QInputDialog::getText(this, tr("World Editor - Server Prompt"), tr("Username ?"));
    QString password = QInputDialog::getText(this, tr("World Editor - Server Prompt"), tr("Password ?"), QLineEdit::Password);
    QString world_database_name = QInputDialog::getText(this, tr("World Editor - Server Prompt"), tr("World Database Name ?"));
    qint32 port = QInputDialog::getInt(this, tr("World Editor - Server Prompt"), tr("Port ?"), 3306);

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", tr("Merge Connection - World Editor"));
    db.setHostName(hostname);
    db.setPort(port);
    db.setDatabaseName(world_database_name);
    db.setUserName(username);
    db.setPassword(password);

    if (!db.open())
    {
        QMessageBox::critical(this, tr("Merge failure"), tr("Connection to database failure."));
        return;
    }

    QString DestinationFileNameDatabase = QFileDialog::getSaveFileName(this, tr("World Editor"), qApp->applicationDirPath(), tr("Client database files(*.dbc)"));
    QFile file(DestinationFileNameDatabase);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        QMessageBox::critical(this, tr("Merge failure"), tr("Connection to client file failure."));

    QSqlQuery query(db);
    QTextStream out(&file);
    QString header;

    // Creature Templates
    header = "CreatureTemplate;";
    if (!query.exec("SELECT * FROM `creature_template`;"))
    {
        QMessageBox::critical(this, tr("Merge failure"), tr("Merge creature template failure."));
    }
    else
    {
        while (query.next())
        {
            out << header;

            out << query.value(0).toULongLong() << ";"; // ID
            out << query.value(1).toString() << ";"; // Name
            out << query.value(2).toString() << ";"; // Tileset
            out << query.value(3).value<uint16>() << ";"; // Tx
            out << query.value(4).value<uint16>() << ";"; // Ty
            out << query.value(5).value<uint16>() << ";"; // Npc flag
            out << query.value(6).value<uint32>() << ";"; // MaxHealth
            out << query.value(7).value<uint32>(); // MaxPower

            out << '\n';
        }
        QMessageBox::information(this, tr("Merge success"), tr("Merge creature template success."));
    }

    // Item Templates
    header = "ItemTemplate;";

    if (!query.exec("SELECT * FROM `item_template`;"))
    {
        QMessageBox::critical(this, tr("Merge failure"), tr("Merge item template failure."));
    }
    else
    {
        while (query.next())
        {
            out << header;

            out << query.value(0).toULongLong() << ";"; // ID
            out << query.value(1).value<uint32>() << ";"; // Class
            out << query.value(2).value<uint32>() << ";"; // SubClass
            out << query.value(3).toString() << ";"; // Name
            out << query.value(4).value<uint16>() << ";"; // DisplayID
            out << query.value(5).value<uint16>() << ";"; // InventoryType
            out << query.value(6).value<uint16>() << ";"; // ContainerSlots
            out << query.value(7).toString(); // Description

            out << '\n';
        }
        QMessageBox::information(this, tr("Merge success"), tr("Merge item template success."));
    }

    // Quest Templates
    header = "QuestTemplate;";

    if (!query.exec("SELECT * FROM `quest_template`;"))
    {
        QMessageBox::critical(this, tr("Merge failure"), tr("Merge quest template failure."));
    }
    else
    {
        while (query.next())
        {
            out << header;

            out << query.value(0).toULongLong() << ";"; // ID
            out << query.value(1).toString() << ";"; // Title
            out << query.value(2).toString() << ";"; // Details
            out << query.value(3).toString(); // Objective

            out << '\n';
        }
        QMessageBox::information(this, tr("Merge success"), tr("Merge quest template success."));
    }

    QMessageBox::information(this, tr("Merge status"), tr("Merge finished."));
}
