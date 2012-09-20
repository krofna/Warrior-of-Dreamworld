#ifndef MESSAGE_CHAT_AREA_DEFINED
#define MESSAGE_CHAT_AREA_DEFINED

#include <vector>
#include <SFML/Graphics.hpp>
#include "../shared/Defines.hpp"

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
