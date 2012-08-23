#include "MessageChatArea.hpp"
#include "World.hpp"

MessageChatArea::MessageChatArea()
{
    if (!m_DefaultFont.loadFromFile("arial.ttf"))
        throw std::runtime_error("Impossible to load arial.ttf font");
}

void MessageChatArea::AddMessage(std::string const& ObjectName, std::string const& Content)
{
    std::string Message = ObjectName + ": " + Content;
    m_Messages.push_back(Message);
}

void MessageChatArea::Draw()
{
    int iMessage = m_Messages.size();
    while (iMessage > (m_Messages.size() - 5))
    {
        std::string Message = m_Messages[iMessage - 1];

        sf::Text text(Message);
        text.setFont(m_DefaultFont);
        text.setPosition(MESSAGE_POS_X * iMessage, MESSAGE_POS_Y * iMessage);

        Window->draw(text);

        --iMessage;
    }
}
