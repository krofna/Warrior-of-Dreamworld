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
#include <cctype>

ChatCommand* CommandHandler::GetCommandTable()
{
    static ChatCommand AccountSetCommandTable[] =
    {
        { "seclevel",   SEC_ADMIN,  true,   &CommandHandler::HandleAccountSetSecLevelCommand,   "Usage: account set seclevel $seclevel(0-4)",NULL },
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
    if((Command.empty()) && (Command[0] != '.') && (!std::isalpha(Command[1])))
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
        for(pCommand = pCommand->ChildCommands; pCommand->Name != NULL; ++pCommand)
        {
            if(Command.compare(pCommand->Name) == 0)
                break;
        }
        ExtractArg(Command);
    }

    if(Console && pCommand->AllowConsole)
    {
        (this->*pCommand->Handler)();
    }
    else 
    {
        // TODO: Check SEC_LEVEL
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

    sDatabase.PExecute("INSERT INTO `players` VALUES (DEFAULT, '%s'; '%s'; 'dg_classm32.gif', 0, 0, 0, 0, 0", Username.c_str(), Password.c_str());
}

void CommandHandler::HandleAccountDeleteCommand()
{
    std::string Username;
    ExtractArg(Username);
    // Find player
    // A FindPlayer(UserName)->Kick() ?
    // Kick player if online
    // sDatabase.PExecute("DELETE FROM `players` WHERE name='%s'", Username.c_str());
}

void CommandHandler::HandleAccountSetSecLevelCommand()
{
    std::string Username;
    ExtractArg(Username);
}
