/*
Warrior of Dreamworld, 2D Massivly Mutiplayer Online Role-playing Game
Copyright (C) 2012 Ryan Lahfa

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
#ifndef MESSAGE_CHAT_AREA_DEFINED
#define MESSAGE_CHAT_AREA_DEFINED

#include <vector>
#include <SFML/Graphics.hpp>
#include "shared/Defines.hpp"

#define MESSAGE_POS_X 10
#define MESSAGE_POS_Y (WindowHeight / 6) * 5

class MessageChatArea
{
    struct Message
    {
        int32 TimeDisplay;
        std::string StringMessage;
        sf::Color DefaultColor;
        bool IsServerMessage;
    };
public:
    MessageChatArea();

    void AddMessage(std::string const& ObjectName, std::string const& Content, int32 Duration = 5);
    void AddServerMessage(std::string const& Message, sf::Color const& color, int32 Duration = 5);
    void AddRawMessage(std::string const& Message, sf::Color const& Color, int32 Duration = 5);
    void Draw(int32 UpdateTime);
    bool HandleTyping(sf::Event event);

private:
    std::string m_Message;
    bool m_IsTyping;
    bool m_StartCharacter;
    std::vector<Message> m_Messages;
    sf::Font m_DefaultFont;
};

#endif
