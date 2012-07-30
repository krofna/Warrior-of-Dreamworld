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
#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <stack>
#include <memory>
#include "../shared/Defines.hpp"

struct Map;
class WorldObject;

/*
TODO: Instead of *50 use map width: see Map.cpp Map::{ctor}
*/

struct PathfinderNode
{
    PathfinderNode* pParent;
    sf::Vector2i Position;
    unsigned G, H;
};

struct CompareNode : public std::binary_function<PathfinderNode*, PathfinderNode*, bool>                                                                                     
{
  bool operator()(const PathfinderNode* first, const PathfinderNode* second) const
  {
    return((first->G + first->H) > (second->G + second->H));
  }
};

class Pathfinder
{
public:
    Pathfinder(WorldObject* pOrigin);

    void Update(Int32 diff);
    void UpdateTarget(WorldObject* pNewTarget);

private:
    enum TileStatus
    {
        UNKNOWN = 0,
        OPEN    = 1,
        CLOSED  = 2
    };

    void GeneratePath();
    bool CheckOrthogonalPathfinderNode(PathfinderNode* pCurrent, PathfinderNode* pAdjacent);
    bool CheckDiagonalPathfinderNode(PathfinderNode* pCurrent, PathfinderNode* pAdjacent);
    void ResetPathfindingGrid();

    std::stack<sf::Vector2i> Path;
    //std::stack<sf::Vector2i> PathToHome; // NYI
    sf::Vector2i Target;
    WorldObject* pOrigin;
    WorldObject* pTarget;
    Map* pMap;
    Int32 MovementCooldown;
};

#endif
