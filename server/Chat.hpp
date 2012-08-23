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
#ifndef CHAT_H
#define CHAT_H

#include "../shared/Defines.hpp"
#include <boost/tokenizer.hpp>

struct ChatCommand;

class ChatHandler
{
public:
    explicit ChatHandler(PlayerPtr pPlayer);

    // @return: true if command was executed
    //          false if its not a command
    // @throws: BadCommand
    bool ParseCommand(std::string& Msg);

private:
    ChatCommand* GetCommandTable();

    // Handlers
    void HandleAccountCreateCommand();
    void HandleAccountDeleteCommand();
    void HandleAccountSetSecLevelCommand();

    static boost::tokenizer<> Tokenizer;
    static boost::tokenizer<>::iterator TokIter;
    void ExtractArg(std::string& Arg);

    PlayerPtr pPlayer;

private:
    class BadCommand {};
};

#include <string>

struct ChatCommand
{
    const char* Name;
    uint8 SecurityLevel;
    bool AllowConsole;
    void (ChatHandler::*Handler)();
    std::string Help;
    ChatCommand* ChildCommands;
};

#endif
