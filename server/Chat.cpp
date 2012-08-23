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
#include "Chat.hpp"
#include <cctype>

ChatCommand* ChatHandler::GetCommandTable()
{
    static ChatCommand AccountSetCommandTable[] =
    {
        { "seclevel",   SEC_ADMIN,  true,   &ChatHandler::HandleAccountSetSecLevelCommand, "Usage: account set seclevel $seclevel(0-4)",NULL },
        { NULL,         0,          false,  NULL,                                       "",                                             NULL }
    };

    static ChatCommand AccountCommandTable[] =
    {
        { "create",     SEC_ADMIN,  true,   &ChatHandler::HandleAccountCreateCommand,   "Usage: account create $username $password",    NULL },
        { "delete",     SEC_ADMIN,  true,   &ChatHandler::HandleAccountDeleteCommand,   "Usage: account delete $username $password",    NULL },
        { "set",        SEC_ADMIN,  true,   NULL,                                       "Usage: account set <what> <new value>",        AccountSetCommandTable },
        { NULL,         0,          false,  NULL,                                       "",                                             NULL }
    };

    static ChatCommand CommandTable[] =
    {
        { "account",    SEC_PLAYER, true,   NULL,                                       "Usage: account <command> <argv>",              AccountCommandTable },
        { NULL,         0,          false,  NULL,                                       "",                                             NULL }
    };

    return CommandTable;
}

// TODO: Rework this, parsing logic is broken
bool ChatHandler::ParseCommand(std::string& Msg)
{
    // Is it a command?
    if((Msg.empty()) && (Msg[0] != '.') && (!std::isalpha(Msg[1])))
        return false;

    std::string Command;

    boost::tokenizer<> Tokenizer(Msg);
    boost::tokenizer<>::iterator TokIter = Tokenizer.begin();
    ExtractArg(Tokenizer, TokIter, Command);

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
        ExtractArg(Tokenizer, TokIter, Command);
    }

    (this->*pCommand->Handler)();

    return true;
}

void ChatHandler::ExtractArg(boost::tokenizer<>& Tokenizer, boost::tokenizer<>::iterator& TokIter, std::string& Arg)
{
    if(TokIter == Tokenizer.end())
        throw BadCommand();

    Arg = *TokIter++;
}

void ChatHandler::HandleAccountCreateCommand()
{
}

void ChatHandler::HandleAccountDeleteCommand()
{
}

void ChatHandler::HandleAccountSetSecLevelCommand()
{
}