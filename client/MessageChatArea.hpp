#ifndef MESSAGE_CHAT_AREA_DEFINED
#define MESSAGE_CHAT_AREA_DEFINED

#include <vector>
#include <SFML/Graphics.hpp>
#include "../shared/Defines.hpp"

#define MESSAGE_POS_X 0
#define MESSAGE_POS_Y (WindowHeight / 6) * 5

class MessageChatArea
{
    struct Message
    {
        int32 TimeDisplay;
        std::string StringMessage;
    };
    public:
    MessageChatArea();

    void AddMessage(std::string const& ObjectName, std::string const& Content, int32 SecondsTime = 5);
    void Draw(int32 UpdateTime);
    bool HandleTyping(sf::Event event);

    private:
    std::string m_Message;
    bool m_IsTyping;
    std::vector<Message> m_Messages;
    sf::Font m_DefaultFont;
};

#endif
