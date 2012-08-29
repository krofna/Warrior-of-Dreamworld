#include "MessageChatArea.hpp"
#include "World.hpp"
#include "WorldSession.hpp"

MessageChatArea::MessageChatArea() : m_IsTyping(false)
{
    if (!m_DefaultFont.loadFromFile("arial.ttf"))
        throw std::runtime_error("Impossible to load arial.ttf font");
}

void MessageChatArea::AddMessage(std::string const& ObjectName, std::string const& Content, int32 SecondsTime)
{
    std::string StringMessage = ObjectName + ": " + Content;
    Message message;

    message.TimeDisplay = SecondsTime * 1000;
    message.StringMessage = StringMessage;

    m_Messages.push_back(message);
}

void MessageChatArea::Draw(int32 UpdateTime)
{
    if (m_IsTyping)
    {
        sf::Text plrText(m_Message);
        plrText.setFont(m_DefaultFont);
        plrText.setPosition(MESSAGE_POS_X, MESSAGE_POS_Y + 50); // Would you take a little more of voodo ?
        Window->draw(plrText);
    }

    if (m_Messages.empty())
        return;

    for (int i = 0 ; i < m_Messages.size() ; ++i)
    {
        if (m_Messages[i].TimeDisplay - UpdateTime <= 0)
        {
            m_Messages.erase(m_Messages.begin() + i);
            i--;
            continue;
        }
        else
            m_Messages[i].TimeDisplay -= UpdateTime;

        std::string StringMessage = m_Messages[i].StringMessage;

        sf::Text text(StringMessage);
        text.setFont(m_DefaultFont);
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
                Session->SendChatMessage(m_Message);
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
