/*
Warrior of Dreamworld, 2D Massivly Mutiplayer Online Role-playing Game
Copyright (C) 2012  Mislav Blazevic

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#include "CommandHandler.hpp"
#include "Database.hpp"
#include "Player.hpp"
#include "ObjectMgr.hpp"
#include "../shared/Log.hpp"
#include "GUID.hpp"
#include <boost/lexical_cast.hpp>
#include <cctype>

#define NullCommand { NULL, 0, false, NULL, "", NULL }

ChatCommand* CommandHandler::GetCommandTable()
{
    static ChatCommand AccountSetCommandTable[] =
    {
        { "seclevel",   SEC_ADMIN,  true,   &CommandHandler::HandleAccountSetSecLevelCommand,   "Usage: account set seclevel $username $seclevel(0-4)", NULL },
        { "password",   SEC_ADMIN,  true,   &CommandHandler::HandleAccountSetPasswordCommand,   "Usage: account set password $username $newpassword", NULL },
        NullCommand
    };

    static ChatCommand AccountCommandTable[] =
    {
        { "create",     SEC_ADMIN,  true,   &CommandHandler::HandleAccountCreateCommand,    "Usage: account create $username $password",    NULL },
        { "delete",     SEC_ADMIN,  true,   &CommandHandler::HandleAccountDeleteCommand,    "Usage: account delete $username $password",    NULL },
        { "set",        SEC_ADMIN,  true,   NULL,                                           "Usage: account set <what> <new value>",        AccountSetCommandTable },
        NullCommand
    };

    static ChatCommand CommandTable[] =
    {
        { "account",    SEC_PLAYER, true,   NULL,                                           "Usage: account <command> <argv>",              AccountCommandTable },
        { "kill",       SEC_ADMIN,  true,   &CommandHandler::HandleKillCommand,             "Usage: kill <player_name>", NULL },
        { "shutdown",   SEC_ADMIN,  true,   &CommandHandler::HandleShutdownCommand,         "Usage: shutdown <time in seconds>" },
        NullCommand
    };

    return CommandTable;
}

bool CommandHandler::ExecuteCommand()
{
    std::string Command;
    ExtractArg(Command);

    // Is it a command?
    if(Command.empty())
        return false;

    if((!Console) && ((Command[0] != '.') || (!std::isalpha(Command[1]))))
        return false;

    // Is there such command?
    ChatCommand* pCommand;
    for(pCommand = GetCommandTable(); pCommand->Name != NULL; ++pCommand)
        if(Command.compare(pCommand->Name) == 0)
            break;

    if(pCommand->Name == NULL)
        throw BadCommand();

    while(!pCommand->Handler)
    {
        ExtractArg(Command);

        if(pCommand->ChildCommands == NULL)
            throw BadCommand();

        for(pCommand = pCommand->ChildCommands; pCommand->Name != NULL; ++pCommand)
        {
            if(Command.compare(pCommand->Name) == 0)
                break;
        }
    }

    if(Console && pCommand->AllowConsole)
    {
        (this->*pCommand->Handler)();
        sLog.Write("Hopefully executed command ...");
    }
    else 
    {
        if (pPlayer->GetSecLevel() >= pCommand->SecurityLevel)
            (this->*pCommand->Handler)();
        else
            return false;
    }

    return true;
}

void CommandHandler::ExtractArg(std::string& Arg)
{
    if(TokIter == Tokenizer.end())
        throw BadCommand();

    Arg = *TokIter++;
}

void CommandHandler::ExtractArg(uint32& Arg)
{
    try
    {
        Arg = boost::lexical_cast<uint32>(*TokIter++);
    }
    catch(boost::bad_lexical_cast& e)
    {
        sLog.Write(e.what());
        throw BadCommand();
    }
}

void CommandHandler::HandleAccountCreateCommand()
{
    std::string Username, Password;
    ExtractArg(Username);
    ExtractArg(Password);

    sDatabase.PExecute("INSERT INTO `players` VALUES (%llu, '%s', '%s', 'none', 0, 0, 'dg_classm32.gif', 0, 0, 0, 0, 0)", Generate64BitsGUID(), Username.c_str(), Password.c_str());
    sLog.Write("Account %s successfully created.", Username.c_str());
}

void CommandHandler::HandleAccountDeleteCommand()
{
    std::string Username;
    ExtractArg(Username);

    Player* pPlayer = sObjectMgr.GetPlayer(Username);
    if (pPlayer->IsInWorld())
        pPlayer->Kick();

    sDatabase.PExecute("DELETE FROM `players` WHERE `username` = '%s'", Username.c_str());
}

void CommandHandler::HandleAccountSetSecLevelCommand()
{
    std::string Username;
    ExtractArg(Username);

    uint32 SecLevel;
    ExtractArg(SecLevel);

    if(SecLevel > SEC_ADMIN || SecLevel < SEC_PLAYER)
        throw BadCommand();

    sDatabase.PExecute("UPDATE `players` SET `seclevel` = %u WHERE `username` = '%s'", SecLevel, Username.c_str());
}

void CommandHandler::HandleAccountSetPasswordCommand()
{
    std::string Username;
    std::string Password;

    ExtractArg(Username);
    ExtractArg(Password);

    sDatabase.PExecute("UPDATE `players` SET `password` = %s WHERE `username` = '%s'", Password.c_str(), Username.c_str());
}

void CommandHandler::HandleKillCommand()
{
    std::string PlayerName;

    ExtractArg(PlayerName);

    Player* pPlayer = sObjectMgr.GetPlayer(PlayerName);
    if (pPlayer)
    {
        if (pPlayer->IsInWorld())
            pPlayer->Kill();
        else
            sLog.Write("Player is not in world !");
    }
    else
        sLog.Write("Unknown player !");
}

void CommandHandler::HandleShutdownCommand()
{
    uint32 time;

    ExtractArg(time);
}
