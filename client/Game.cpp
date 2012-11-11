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
#include "Game.hpp"
#include <boost/bind.hpp>

boost::asio::io_service* Game::sIO = nullptr;

void Game::Initialize(boost::asio::io_service* io)
{
    sIO = io;

    Game* pInstance = GetInstance();

    pInstance->Window.reset(new sf::RenderWindow);
    pInstance->SFGUI.reset(new sfg::SFGUI);
    pInstance->Desktop.reset(new sfg::Desktop);

    pInstance->GetRenderWindow()->create(*sf::VideoMode::getFullscreenModes().begin(), "Warrior of Dreamworld", sf::Style::Close);
    pInstance->GetRenderWindow()->setFramerateLimit(60);
}

Game::~Game()
{
    // 1. We need to pop all states, after let RenderWindow, SFGUI and Desktop objects destroy by themself
    PopAllStates();
}

boost::asio::io_service* Game::GetIO()
{
    return sIO;
}

std::shared_ptr<sf::RenderWindow> Game::GetRenderWindow()
{
    return Window;
}

std::shared_ptr<sfg::SFGUI> Game::GetSFGUI()
{
    return SFGUI;
}

std::shared_ptr<sfg::Desktop> Game::GetDesktop()
{
    return Desktop;
}

void Game::Update()
{
    if(!Window->isOpen())
    {
        sIO->stop();
        return;
    }

    while(Window->pollEvent(Event))
    {
        StateStack.top()->HandleEvent(Event);
    }
    Window->clear();
    StateStack.top()->Draw();
    Window->display();
    StateStack.top()->Update();

    sIO->post(boost::bind(&Game::Update, this));
}

void Game::PushState(GameState* pState)
{
    StateStack.push(std::unique_ptr<GameState>(pState));
}

void Game::PopState()
{
    if(!StateStack.empty())
        StateStack.pop();
}

void Game::PopAllStates()
{
    while(!StateStack.empty())
        StateStack.pop();
}
