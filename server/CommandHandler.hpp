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
#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

#include "../shared/Defines.hpp"
#include <boost/tokenizer.hpp>

struct ChatCommand;

class CommandHandler
{
public:
    CommandHandler(std::string& Msg) : Console(true), Tokenizer(Msg), TokIter(Tokenizer.begin()) { }
    CommandHandler(PlayerPtr& pPlayer, std::string Msg) : pPlayer(pPlayer), Console(false), Tokenizer(Msg), TokIter(Tokenizer.begin()) { }

    // @return: true if command was executed
    //          false if its not a command
    // @throws: BadCommand
    bool ExecuteCommand();

private:
    ChatCommand* GetCommandTable();

    // Handlers
    void HandleAccountCreateCommand();
    void HandleAccountDeleteCommand();
    void HandleAccountSetSecLevelCommand();

    void ExtractArg(std::string& Arg);

    PlayerPtr pPlayer;
    boost::tokenizer<> Tokenizer;
    boost::tokenizer<>::iterator TokIter;
    bool Console;

public:
    class BadCommand {};
};

#include <string>

struct ChatCommand
{
    const char* Name;
    uint8 SecurityLevel;
    bool AllowConsole;
    void (CommandHandler::*Handler)();
    std::string Help;
    ChatCommand* ChildCommands;
};

#endif
