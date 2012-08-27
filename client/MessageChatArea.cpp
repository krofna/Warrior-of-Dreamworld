#include "MessageChatArea.hpp"
#include "World.hpp"

MessageChatArea::MessageChatArea()
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
