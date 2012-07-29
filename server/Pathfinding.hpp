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
#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <stack>
#include <memory>
#include "../shared/Defines.hpp"

struct Tile;
struct Map;

namespace pathfinding
{
    enum TileStatus
    {
        OPEN    = 0,
        CLOSED  = 1,
        UNKNOWN = 2
    };

    struct Node
    {
        Node() 
        {
             G = H = 0;
        }

        Node* pParent;
        sf::Vector2i Position;
        int G, H;
        int Status;

        bool operator < (const Node& first) const
        {
            return((this->G + this->H) < (first.G + first.H));
        }
    };

    typedef std::unique_ptr<std::stack<sf::Vector2i> > Path;

    Path GeneratePath(Map* pMap, sf::Vector2i pOrigin, sf::Vector2i pTarget);
    bool CheckOrthogonalNode(Node* pCurrent, Node* pAdjacent);
    bool CheckDiagonalNode(Node* pCurrent, Node* pAdjacent);
    void ResetPathfindingGrid(Map* pMap);

    extern sf::Vector2i Target;
}

#endif