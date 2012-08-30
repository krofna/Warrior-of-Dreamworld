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
#include "Animation.hpp"
#include "Globals.hpp"
#include <cmath>

void Animation::Load(WorldPacket Argv)
{
    sf::Vector2f Position;
    Argv.ReadSkip<uint16>(); // Effect
    Argv.ReadSkip<uint16>(); // DisplayID
    Argv >> Angle >> ID >> Position.x >> Position.y;
    Sprite.setTexture(*sObjectMgr->GetTileset("t.png"));
    Sprite.setPosition(Position);
    Time = sf::microseconds(0);
}

void Animation::Update()
{
    Time += Clock.getElapsedTime();
    Clock.restart();
    int Multiplier = Time.asMilliseconds() / 15;
    Time -= sf::milliseconds(15 * Multiplier);

    Sprite.move(std::sin(this->Angle) * Multiplier, std::cos(this->Angle) * Multiplier);
    Window->draw(Sprite);
}
