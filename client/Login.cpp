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
#include "Login.hpp"
#include "Globals.hpp"
#include "WorldSession.hpp"
#include "../shared/Opcodes.hpp"

Login::Login() : m_CurrentFocus(None)
{
    Window->resetGLStates();
    Create();
}
Login::~Login()
{
    sDesktop.Remove(m_LoginWindow);
}

void Login::HandleEvent(sf::Event Event)
{
    sDesktop.HandleEvent(Event);
    if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Tab)
        GrabNextFocus();
    if (Event.type == sf::Event::KeyPressed && Event.key.code == sf::Keyboard::Return)
        onConnectButtonPressed();
}

void Login::Draw()
{
    sSFGUI->Display(*Window);
}

void Login::Update()
{
    sDesktop.Update(0); // FIXME: Use a good frame time !
}

void Login::Create()
{
    m_LoginWindow = sfg::Window::Create();
    m_LoginWindow->SetTitle("Login");

    sfg::Box::Ptr Global = sfg::Box::Create(sfg::Box::VERTICAL);
    sfg::Box::Ptr IP = sfg::Box::Create(sfg::Box::HORIZONTAL);
    sfg::Box::Ptr Port = sfg::Box::Create(sfg::Box::HORIZONTAL);
    sfg::Box::Ptr Username = sfg::Box::Create(sfg::Box::HORIZONTAL);
    sfg::Box::Ptr Password = sfg::Box::Create(sfg::Box::HORIZONTAL);

    m_ConnectButton = sfg::Button::Create();
    m_ConnectButton->SetLabel("Connect!");

    m_ConnectButton->GetSignal(sfg::Widget::OnLeftClick).Connect(&Login::onConnectButtonPressed, this);

    m_CurrentState = sfg::Label::Create("Wait for user...");

    m_IPEntry = sfg::Entry::Create();
    m_IPEntry->SetRequisition(sf::Vector2f(80.f, 0.f));

    m_PortEntry = sfg::Entry::Create();
    m_PortEntry->SetRequisition(sf::Vector2f(40.f, 0.f));

    m_UsernameEntry = sfg::Entry::Create();
    m_UsernameEntry->SetRequisition(sf::Vector2f(85.f, 0.f));

    m_PasswordEntry = sfg::Entry::Create();
    m_PasswordEntry->SetRequisition(sf::Vector2f(90.f, 0.f));
    m_PasswordEntry->HideText('*');

    IP->Pack(sfg::Label::Create("IP Address: "));
    IP->Pack(m_IPEntry);

    Port->Pack(sfg::Label::Create("Port: "));
    Port->Pack(m_PortEntry);

    Username->Pack(sfg::Label::Create("Username: "));
    Username->Pack(m_UsernameEntry);

    Password->Pack(sfg::Label::Create("Password: "));
    Password->Pack(m_PasswordEntry);

    Global->Pack(IP);
    Global->Pack(Port);
    Global->Pack(Username);
    Global->Pack(Password);
    Global->Pack(m_ConnectButton);
    Global->Pack(m_CurrentState);

    Global->SetSpacing(5.f);

    m_LoginWindow->Add(Global);
    m_LoginWindow->SetPosition(sf::Vector2f(WindowWidth / 2, WindowHeight / 2));

    sDesktop.Add(m_LoginWindow);
}

void Login::onConnectButtonPressed()
{
    Session->SendAuthRequest(m_UsernameEntry->GetText(), m_PasswordEntry->GetText());
    m_CurrentState->SetText("Logging in progress...");
}
void Login::GrabNextFocus()
{
    switch (m_CurrentFocus)
    {
        case IP:
        {
            m_CurrentFocus = Port;
            m_PortEntry->GrabFocus();
            break;
        }
        case Port:
        {
            m_CurrentFocus = Username;
            m_UsernameEntry->GrabFocus();
            break;
        }
        case Username:
        {
            m_CurrentFocus = Password;
            m_PasswordEntry->GrabFocus();
            break;
        }
        case Password:
        {
            m_CurrentFocus = IP;
            m_IPEntry->GrabFocus();
            break;
        }
        case None:
        {
            m_CurrentFocus = IP;
            m_IPEntry->GrabFocus();
            break;
        }
        default:
        {
            sLog.Write("Unknown focus.");
        }
    }
}
