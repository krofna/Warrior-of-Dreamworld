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

#define NullCommand { nullptr, 0, false, nullptr, "", nullptr }

ChatCommand* CommandHandler::GetCommandTable()
{
    static ChatCommand AccountSetCommandTable[] =
    {
        { "seclevel",   SEC_ADMIN,  true,   &CommandHandler::HandleAccountSetSecLevelCommand,   "Usage: account set seclevel $username $seclevel",      nullptr },
        { "password",   SEC_PLAYER, true,   &CommandHandler::HandleAccountSetPasswordCommand,   "Usage: account set password $username $newpassword",   nullptr },
        NullCommand
    };

    static ChatCommand AccountCommandTable[] =
    {
        { "create",     SEC_ADMIN,  true,   &CommandHandler::HandleAccountCreateCommand,    "Usage: account create $username $password",    nullptr },
        { "delete",     SEC_ADMIN,  true,   &CommandHandler::HandleAccountDeleteCommand,    "Usage: account delete $username $password",    nullptr },
        { "set",        SEC_PLAYER, true,   nullptr,                                        "Usage: account set <what> <new value>",        AccountSetCommandTable },
        NullCommand
    };

    static ChatCommand TeleportCommandTable[] =
    {
        { "to",         SEC_MOD,    false,   &CommandHandler::HandleTeleportToCommand,      "Usage: teleport to $username",                 nullptr },
        { "at",         SEC_MOD,    false,   &CommandHandler::HandleTeleportAtCommand,      "Usage: teleport at $x $y",                     nullptr },
        NullCommand
    };

    static ChatCommand CommandTable[] =
    {
        { "account",    SEC_PLAYER, true,   nullptr,                                        "Usage: account <command> <argv>",              AccountCommandTable },
        { "kill",       SEC_ADMIN,  true,   &CommandHandler::HandleKillCommand,             "Usage: kill <player_name>",                    nullptr },
        { "shutdown",   SEC_ADMIN,  true,   &CommandHandler::HandleShutdownCommand,         "Usage: shutdown <time in seconds>",            nullptr },
        { "teleport",   SEC_ADMIN,  false,  nullptr,                                        "Usage: teleport <subcommand>",                 TeleportCommandTable },
        { "bring",      SEC_ADMIN,  false,  &CommandHandler::HandleBringCommand,            "Usage: bring <player_name>",                   nullptr },
        { "help",       SEC_PLAYER, true,   &CommandHandler::HandleHelpCommand,             "Usage: help <command>",                        nullptr },
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
    for(pCommand = GetCommandTable(); pCommand->Name != nullptr; ++pCommand)
        if(Command.compare(pCommand->Name) == 0)
            break;

    if(pCommand->Name == nullptr)
        throw BadCommand();

    while(!pCommand->Handler)
    {
        ExtractArg(Command);

        if(pCommand->ChildCommands == nullptr)
            throw BadCommand();

        for(pCommand = pCommand->ChildCommands; pCommand->Name != nullptr; ++pCommand)
        {
            if(Command.compare(pCommand->Name) == 0)
                break;
        }
    }

    if(Console)
    {
        if(pCommand->AllowConsole)
        {
            (this->*pCommand->Handler)();
        }
        else
        {
            sLog.Write("You can't do this from console!");
        }
    }
    else 
    {
        if (pPlayer->GetSecLevel() >= pCommand->SecurityLevel)
        {
            (this->*pCommand->Handler)();
        }
        else
        {
            pPlayer->SendCommandReponse("You don't have the level for executing this command!");
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
        pPlayer->LogOut();

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
                sLog.Write("Player is not in world!");
            else
                pPlayer->SendCommandReponse("Player is not in world!");
        }
    }
    else
    {
        if (Console)
            sLog.Write("Unknown player!");
        else
            pPlayer->SendCommandReponse("Unknown player!");
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
    {
        pPlayer->SendCommandReponse("Player doesn't exist !");
    }
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
    uint32 Time;

    ExtractArg(Time);
}

void CommandHandler::HandleHelpCommand()
{
    std::string Command, Help;
    ExtractArg(Command);

    ChatCommand* pCommand = SubCommandHelper(Command);

    if (pCommand == nullptr)
    {
        if (Console)
            sLog.Write("There is not a such command !");
        else
            pPlayer->SendCommandReponse("There is not a such command !");

        return;
    }
    else
        Help = pCommand->Help;

    if (Console)
        sLog.Write("%s", Help.c_str());
    else
        pPlayer->SendCommandReponse(Help);
}

ChatCommand* CommandHandler::SubCommandHelper(std::string const& CommandBaseName)
{
    ChatCommand* pCommand;
    for (pCommand = GetCommandTable() ; pCommand->Name != nullptr ; ++pCommand)
        if (pCommand->Name == CommandBaseName)
            break;

    if (pCommand->Name == nullptr)
        return nullptr;

    if (pCommand->ChildCommands == nullptr)
        return pCommand;

    else if (!IsEndArgument())
    {
        while (!IsEndArgument())
        {
            std::string SubCommand;
            ExtractArg(SubCommand);
            
            for (pCommand = pCommand->ChildCommands ; pCommand->Name != nullptr ; ++pCommand)
                if (pCommand->Name == SubCommand)
                    break;
            
            if (pCommand->Name == nullptr)
                return nullptr;

            if (pCommand->ChildCommands == nullptr)
                return pCommand;
        }
    }

    return pCommand;
}

bool CommandHandler::IsEndArgument() const
{
    return TokIter == Tokenizer.end();
}
