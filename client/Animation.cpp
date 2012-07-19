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
#include <cmath>
#include "Math.h"

// TODO: [PH]
Animation::Animation(sf::Vector2f Position, float Angle) :
Angle               (Angle),
Index               (0),
Position            (Position)
{
    t.loadFromFile("t.png");
    Sprites.push_back(sf::Sprite(t));
    Sprites.begin()->setPosition(Position);
}

// TODO: [PH]
void Animation::Update()
{
    if(Clock.getElapsedTime() > sf::milliseconds(1))
    {
        // PH, implement Animation speed
        Position.x += std::sin(this->Angle);
        Position.y += std::cos(this->Angle);
        Sprites.begin()->setPosition(this->Position);
        Clock.restart();
    }
    Sprites.begin()->setTexture(t); // hack
    Window.draw(Sprites[Index]);
}