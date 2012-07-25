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
#include "ResourceManager.h"

// TODO: [PH]
Animation::Animation(Uint16 DisplayID, sf::Vector2f Position, float Angle, Uint32 ID) :
Angle               (Angle),
Sprite              (*ResourceManager::GetTileset("t.png")),
ID                  (ID),
Time                (sf::microseconds(0))
//DisplayID         (Do something with it)
//Speed
{
    Sprite.setPosition(Position);
}

void Animation::Update()
{
    Time += Clock.getElapsedTime();
    Clock.restart();
    int Multiplier = Time.asMilliseconds() / 15;
    Time -= sf::milliseconds(15 * Multiplier);

    Sprite.move(std::sin(this->Angle) * Multiplier, std::cos(this->Angle) * Multiplier);
    Window.draw(Sprite);
}