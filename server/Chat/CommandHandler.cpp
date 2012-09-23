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
#include <boost/lexical_cast.hpp>

#include "CommandHandler.hpp"
#include "Database.hpp"
#include "Player.hpp"
#include "ObjectMgr.hpp"
#include "shared/Log.hpp"
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

    static ChatCommand TeleportCommandTable[] =
    {
        { "to",         SEC_ADMIN,  false,   &CommandHandler::HandleTeleportToCommand,       "Usage: teleport to <player_name>",             NULL },
        { "at",         SEC_ADMIN,  false,   &CommandHandler::HandleTeleportAtCommand,       "Usage: teleport at <x> <y>",                   NULL },
        NullCommand
    };

    static ChatCommand CommandTable[] =
    {
        { "account",    SEC_PLAYER, true,   NULL,                                           "Usage: account <command> <argv>",              AccountCommandTable },
        { "kill",       SEC_ADMIN,  true,   &CommandHandler::HandleKillCommand,             "Usage: kill <player_name>",                    NULL },
        { "shutdown",   SEC_ADMIN,  true,   &CommandHandler::HandleShutdownCommand,         "Usage: shutdown <time in seconds>",            NULL },
        { "teleport",   SEC_ADMIN,  false,  NULL,                                           "Usage: teleport <subcommand>",                 TeleportCommandTable },
        { "bring",      SEC_ADMIN,  false,  &CommandHandler::HandleBringCommand,            "Usage: bring <player_name>",                   NULL },
        { "help",       SEC_PLAYER, true,   &CommandHandler::HandleHelpCommand,             "Usage: help <command>",                        NULL },
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
        {
            (this->*pCommand->Handler)();
        }
        else
        {
            pPlayer->SendCommandReponse("You don't have the level for executing this command !");
            return false;
        }
    }

    return true;
}

template<>
void CommandHandler::ExtractArg<std::string>(std::string& Arg)
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

    sDatabase.PExecute("INSERT INTO `players` VALUES (%llu, '%s', '%s', 'none', 0, 0, 'dg_classm32.gif', 0, 0, 0, 0, 0)", sDatabase.Generate64BitsGUID(), Username.c_str(), Password.c_str());
    if (Console)
        sLog.Write("Account %s successfully created.", Username.c_str());
    else
    {
        std::string Msg = "Account ";
        Msg += Username;
        Msg += " successfully created.";

        pPlayer->SendCommandReponse(Msg);
    }
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

    if (Console)
        ExtractArg(Username);
    else
        Username = pPlayer->GetUsername();

    ExtractArg(Password);

    sDatabase.PExecute("UPDATE `players` SET `password` = '%s' WHERE `username` = '%s'", Password.c_str(), Username.c_str());
}

void CommandHandler::HandleKillCommand()
{
    std::string PlayerName;
    ExtractArg(PlayerName);

    Player* pTargetPlayer = sObjectMgr.GetPlayer(PlayerName);
    if (pTargetPlayer)
    {
        if (pTargetPlayer->IsInWorld())
            pTargetPlayer->Kill();
        else
        {
            if (Console)
                sLog.Write("Player is not in world !");
            else
                pPlayer->SendCommandReponse("Player is not in world !");
        }
    }
    else
    {
        if (Console)
            sLog.Write("Unknown player !");
        else
            pPlayer->SendCommandReponse("Unknown player !");
    }
}

void CommandHandler::HandleTeleportToCommand()
{
    std::string PlayerName;
    ExtractArg(PlayerName);

    Player* pTargetPlayer = sObjectMgr.GetPlayer(PlayerName);
    if (pTargetPlayer)
    {
        if (pTargetPlayer->IsInWorld())
        {
            pPlayer->Teleport(pTargetPlayer->GetPosition());
        }
        else
        {
            pPlayer->SendCommandReponse("Player is not in world !");
        }
    }
    else
        pPlayer->SendCommandReponse("Player doesn't exist !");
}

void CommandHandler::HandleTeleportAtCommand()
{
    int x, y;
    ExtractArg(x);
    ExtractArg(y);

    pPlayer->Teleport(x, y);
}

void CommandHandler::HandleBringCommand()
{
    std::string PlayerName;
    ExtractArg(PlayerName);

    Player* pTargetPlayer = sObjectMgr.GetPlayer(PlayerName);

    if (pTargetPlayer)
    {
        if (pTargetPlayer->IsInWorld())
        {
            pTargetPlayer->Teleport(pPlayer->GetPosition());
        }
        else
        {
            pPlayer->SendCommandReponse("Player is not in world !");
        }
    }
    else
        pPlayer->SendCommandReponse("Player doesn't exist !");
}

void CommandHandler::HandleShutdownCommand()
{
    uint32 time;

    ExtractArg(time);
}

void CommandHandler::HandleHelpCommand()
{
    std::string Command, Help;
    ExtractArg(Command);

    // Is this command exists ?
    ChatCommand* pCommand;
    for (pCommand = GetCommandTable() ; pCommand->Name != NULL ; ++pCommand)
    {
        if (Command == pCommand->Name)
        {
            Help = pCommand->Help;
            break;
        }
    }

    if (pCommand == NULL)
    {
        if (Console)
            sLog.Write("There is not a such command !");
        else
            pPlayer->SendCommandReponse("There is not a such command !");
    }

    if (Console)
        sLog.Write("%s", Help);
    else
        pPlayer->SendCommandReponse(Help);
}