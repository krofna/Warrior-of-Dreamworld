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
#include "Animation.h"
#include "Globals.h"

// TODO: [PH]
Animation::Animation(float x, float y, Uint8 Direction) :
Direction           (Direction)
{
    t.loadFromFile("testspell.png");
    sf::Sprite* pSprite = new sf::Sprite();
    pSprite->setTexture(t);
    pSprite->setPosition(x, y);
    Sprites.push_back(pSprite);
}

// TODO: [PH]
void Animation::Update()
{
    if(Clock.getElapsedTime() > sf::seconds(1))
    {
        switch(Direction)
        {
        case MOVE_UP:
        case MOVE_DOWN:
        case MOVE_LEFT:
        case MOVE_RIGHT:
            (**Sprites.begin()).move(32, 0);
        }
        Clock.restart();
    }

    Window.draw(**Sprites.begin());
}