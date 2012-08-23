#ifndef MESSAGE_CHAT_AREA_DEFINED
#define MESSAGE_CHAT_AREA_DEFINED

#include <vector>
#include <SFML/Graphics.hpp>

#define MESSAGE_POS_X 0
#define MESSAGE_POS_Y (WindowHeight / 6) * 5

class MessageChatArea
{
    public:
    MessageChatArea();

    void AddMessage(std::string const& ObjectName, std::string const& Content);
    void Draw();

    private:
    std::vector<std::string> m_Messages;
    sf::Font m_DefaultFont;
};

#endif
