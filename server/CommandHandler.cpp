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
#include "../shared/Log.hpp"
#include "GUID.hpp"
#include <cctype>

ChatCommand* CommandHandler::GetCommandTable()
{
    static ChatCommand AccountSetCommandTable[] =
    {
        { "seclevel",   SEC_ADMIN,  true,   &CommandHandler::HandleAccountSetSecLevelCommand,   "Usage: account set seclevel $username $seclevel(0-4)",NULL },
        { "password",   SEC_ADMIN,  true,   &CommandHandler::HandleAccountSetPasswordCommand,   "Usage: account set password $username $newpassword", NULL },
        { NULL,         0,          false,  NULL,                                               "",                                          NULL }
    };

    static ChatCommand AccountCommandTable[] =
    {
        { "create",     SEC_ADMIN,  true,   &CommandHandler::HandleAccountCreateCommand,    "Usage: account create $username $password",    NULL },
        { "delete",     SEC_ADMIN,  true,   &CommandHandler::HandleAccountDeleteCommand,    "Usage: account delete $username $password",    NULL },
        { "set",        SEC_ADMIN,  true,   NULL,                                           "Usage: account set <what> <new value>",        AccountSetCommandTable },
        { NULL,         0,          false,  NULL,                                           "",                                             NULL }
    };

    static ChatCommand CommandTable[] =
    {
        { "account",    SEC_PLAYER, true,   NULL,                                           "Usage: account <command> <argv>",              AccountCommandTable },
        { NULL,         0,          false,  NULL,                                           "",                                             NULL }
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

void CommandHandler::HandleAccountCreateCommand()
{
    std::string Username, Password;
    ExtractArg(Username);
    ExtractArg(Password);

    sDatabase.PExecute("INSERT INTO `players` VALUES (%u, '%s', '%s', 0, 0, 'dg_classm32.gif', 0, 0, 0, 0, 0)", Generate64BitsGUID(), Username.c_str(), Password.c_str());

    sLog.Write("Account %s successfully created.", Username.c_str());
}

void CommandHandler::HandleAccountDeleteCommand()
{
    std::string Username;
    ExtractArg(Username);

/*    if (IsConnected)
        pWorld->KickPlayer(GUID);*/

    sDatabase.PExecute("DELETE FROM `players` WHERE `username` = '%s'", Username.c_str());
}

void CommandHandler::HandleAccountSetSecLevelCommand()
{
    std::string Username;
    ExtractArg(Username);

    // TODO: Use a integer for SecLevel.
    std::string SecLevel;
    ExtractArg(SecLevel);

    sDatabase.PExecute("UPDATE `players` SET `seclevel` = %s WHERE `username` = '%s'", SecLevel.c_str(), Username.c_str());
}

void CommandHandler::HandleAccountSetPasswordCommand()
{
    std::string Username;
    std::string Password;

    ExtractArg(Username);
    ExtractArg(Password);

    sDatabase.PExecute("UPDATE `players` SET `password` = %s WHERE `username` = '%s'", Password.c_str(), Username.c_str());
}
