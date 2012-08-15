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
#ifndef WORLD_OBJECT_H
#define WORLD_OBJECT_H

#include "../shared/Defines.hpp"
#include "Loadable.h"
#include "Globals.hpp"
/*
TODO: Maybe everything should be WorldObject as far as client is concerned?
*/

class WorldObject : public Loadable
{
public:
    WorldObject(std::string Name, uint16 x, uint16 y, uint16 tx, uint16 ty);
    virtual ~WorldObject(){}
    virtual void Draw();
    virtual void Load(char* Argv);

    void UpdateCoordinates(uint16 x, uint16 y);
    sf::Vector2f GetPosition();

    std::string GetObjectName()
    {
        return ObjectName;
    }

private:
    sf::Sprite ObjectSprite;
    std::string ObjectName;
};

#endif