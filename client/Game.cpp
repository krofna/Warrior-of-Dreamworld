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

sf::RenderWindow* Window;

Game& Game::GetInstance()
{
    static Game instance;
    return instance;
}

void Game::Create(boost::asio::io_service& io)
{
    GetInstance().io = &io;
    Window = new sf::RenderWindow();
    Window->create(sf::VideoMode(WindowWidth, WindowHeight), "Warrior of Dreamworld", sf::Style::Close);
    Window->setFramerateLimit(60);
}

Game::~Game()
{
    PopAllStates();
    delete Window;
}

void Game::Update()
{
    sf::Event Event;

    while(Window->pollEvent(Event))
    {
        StateStack.top()->HandleEvent(Event);
    }
    Window->clear();
    StateStack.top()->Draw();
    Window->display();
    StateStack.top()->Update();

    io->post(boost::bind(&Game::Update, this));
}

void Game::PushState(GameState* pState)
{
    StateStack.push(pState);
}

void Game::PopState()
{
    if(!StateStack.empty())
    {
        delete StateStack.top();
        StateStack.pop();
    }
}

void Game::PopAllStates()
{
    while(!StateStack.empty())
    {
        PopState();
    }
}
