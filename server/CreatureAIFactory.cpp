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
#include "CreatureAIFactory.hpp"
#include "CreatureAI.hpp"
#include "Creature.hpp"

void CreatureAIFactory::RegisterAI(const std::string& AIName, CreatureAI*(*Creator)(CreaturePtr pCreature))
{
    Registry[AIName] = Creator;
}

CreatureAI* CreatureAIFactory::CreateAI(const std::string& AIName, CreaturePtr pCreature)
{
    if(Registry.find(AIName) == Registry.end())
        return new CreatureAI(pCreature);

    return Registry[AIName](pCreature);
}
