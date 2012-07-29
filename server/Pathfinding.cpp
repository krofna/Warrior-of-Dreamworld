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
#include "Pathfinding.h"
#include "Map.hpp"
#include "../shared/Math.hpp"
#include <queue>

/*
TODO:
- Bounds checking
- Diagonal movement ( no cutting )
- Reuse some code. There is alot of repeating
*/

sf::Vector2i pathfinding::Target;

pathfinding::Path pathfinding::GeneratePath(Map* pMap, sf::Vector2i Origin, sf::Vector2i Target)
{
    pathfinding::Target = Target;

    ResetPathfindingGrid(pMap);

    Node** PathfindingGrid = pMap->PathfindingGrid;
    Node* pOriginNode = &PathfindingGrid[Origin.y][Origin.x];
    pOriginNode->Status = OPEN;
    pOriginNode->pParent = nullptr;

    std::priority_queue<Node*> OpenList;
    OpenList.push(pOriginNode);

    std::vector<std::vector<WorldObject*> >* pTileGrid = &pMap->TileGrid;

    do
    {
        Node* pCurrent = OpenList.top();
        OpenList.pop();
        pCurrent->Status = CLOSED;

        if(pCurrent->Position == Target)
        {
            Path PathToTarget;
            while(pCurrent->pParent != nullptr)
            {
                PathToTarget->push(std::move(pCurrent->Position));
                pCurrent = pCurrent->pParent;
            }
        }

        // Upper
        if((*pTileGrid)[pCurrent->Position.y-1][pCurrent->Position.x] == nullptr)
        {
            Node* pAdjacent = &PathfindingGrid[pCurrent->Position.y-1][pCurrent->Position.x];
            if(CheckOrthogonalNode(pCurrent, pAdjacent))
            {
                OpenList.push(pAdjacent);
            }
        }

        // Lower
        if((*pTileGrid)[pCurrent->Position.y+1][pCurrent->Position.x] == nullptr)
        {
            Node* pAdjacent = &PathfindingGrid[pCurrent->Position.y-1][pCurrent->Position.x];
            if(CheckOrthogonalNode(pCurrent, pAdjacent))
            {
                OpenList.push(pAdjacent);
            }
        }

        // Right
        if((*pTileGrid)[pCurrent->Position.y][pCurrent->Position.x+1] == nullptr)
        {
            Node* pAdjacent = &PathfindingGrid[pCurrent->Position.y-1][pCurrent->Position.x];
            if(CheckOrthogonalNode(pCurrent, pAdjacent))
            {
                OpenList.push(pAdjacent);
            }
        }

        // Left
        if((*pTileGrid)[pCurrent->Position.y-1][pCurrent->Position.x] == nullptr)
        {
            Node* pAdjacent = &PathfindingGrid[pCurrent->Position.y-1][pCurrent->Position.x];
            if(CheckOrthogonalNode(pCurrent, pAdjacent))
            {
                OpenList.push(pAdjacent);
            }
        }
    } while(!OpenList.empty());

    return Path();
}

bool pathfinding::CheckOrthogonalNode(Node* pCurrent, Node* pAdjacent)
{
    switch(pAdjacent->Status)
    {
    case UNKNOWN:
        pAdjacent->pParent = pCurrent;
        pAdjacent->G = pCurrent->G + 10;
        pAdjacent->H = math::GetManhattanDistance(sf::Vector2i(pAdjacent->Position.y, pAdjacent->Position.x), Target);
        pAdjacent->Status = OPEN;
        return true;
        break;

    case OPEN:
        if((pCurrent->G + 10) < pAdjacent->G)
        {
            pAdjacent->pParent = pCurrent;
            pAdjacent->G = pCurrent->G + 10;
            return true;
        }
        break;

    case CLOSED:
        break;
    }

    return false;
}

void pathfinding::ResetPathfindingGrid(Map* pMap)
{
    std::memset(pMap->PathfindingGrid, UNKNOWN, 50 * 50 * sizeof(pMap->PathfindingGrid[0][0]));
}