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
#include "MessageChatArea.hpp"
#include "World.hpp"
#include "WorldSession.hpp"

MessageChatArea::MessageChatArea() :
m_IsTyping                      (false)
{
    if (!m_DefaultFont.loadFromFile("data/fonts/arial.ttf"))
        throw std::runtime_error("Impossible to load arial.ttf font");
}

void MessageChatArea::AddMessage(std::string const& ObjectName, std::string const& Content, int32 SecondsTime)
{
    std::string StringMessage = ObjectName + ": " + Content;
    Message message;

    message.TimeDisplay = SecondsTime * 1000;
    message.StringMessage = StringMessage;
    message.IsServerMessage = false;
    message.DefaultColor = sf::Color::Black;

    m_Messages.push_back(message);
}

void MessageChatArea::AddServerMessage(std::string const& Msg, sf::Color const& msgColor, int32 DisplayTime)
{
    Message message;

    message.TimeDisplay = DisplayTime * 1000;
    message.StringMessage = Msg;
    message.IsServerMessage = true;
    message.DefaultColor = msgColor;

    m_Messages.push_back(message);
}

void MessageChatArea::AddRawMessage(std::string const& Msg, sf::Color const& MsgColor, int32 DisplayTime)
{
    Message message;

    message.TimeDisplay = DisplayTime * 1000;
    message.StringMessage = Msg;
    message.IsServerMessage = false;
    message.DefaultColor = MsgColor;

    m_Messages.push_back(message);
}

void MessageChatArea::Draw(int32 UpdateTime)
{
    if (m_IsTyping)
    {
        sf::Text plrText;
        plrText.setString(m_Message);
        plrText.setFont(m_DefaultFont);
        plrText.setPosition(MESSAGE_POS_X, MESSAGE_POS_Y + 50); // Would you take a little more of voodo ?
        Window->draw(plrText);
    }

    if (m_Messages.empty())
        return;

    for (unsigned i = 0 ; i < m_Messages.size() ; ++i)
    {
        if (m_Messages[i].TimeDisplay - UpdateTime <= 0)
        {
            m_Messages.erase(m_Messages.begin() + i);
            i--;
            continue;
        }
        else
        {
            m_Messages[i].TimeDisplay -= UpdateTime;
        }
        
        std::string StringMessage;
        if (m_Messages[i].IsServerMessage)
            StringMessage += "SERVER: ";
        StringMessage += m_Messages[i].StringMessage;

        sf::Text text(StringMessage, m_DefaultFont);
        text.setColor(m_Messages[i].DefaultColor);
        text.setPosition(MESSAGE_POS_X, MESSAGE_POS_Y - float(i * 50));

        Window->draw(text);
    }
}

bool MessageChatArea::HandleTyping(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::BackSpace)
            m_Message.erase(m_Message.end() - 1);
    }

    if (event.type == sf::Event::TextEntered && m_IsTyping)
    {
        if (!m_StartCharacter)
        {
            if (event.text.unicode != 8)
                m_Message += event.text.unicode;
        }
        else
            m_StartCharacter = false;
    }

    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Return)
        {
            if (m_IsTyping)
            {
                WorldSession::GetInstance()->SendChatMessage(m_Message);
                m_Message.clear();
                m_IsTyping = false;
            }
            else
            {
                m_IsTyping = true;
                m_StartCharacter = true;
            }
        }
    }

    return m_IsTyping;
}
