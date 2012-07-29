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
#include "Pathfinder.hpp"
#include "Map.hpp"
#include "WorldObject.hpp"
#include "../shared/Math.hpp"
#include <queue>

/*
TODO:
- Bounds checking
- Diagonal movement ( no cutting )
- Reuse some code. There is alot of repeating
*/

Pathfinder::Pathfinder(WorldObject* pOrigin) :
pOrigin               (pOrigin),
pMap                  (pOrigin->GetMap())
{
    MovementCooldown = 0;
}

void Pathfinder::Update(Int32 diff)
{
    if(!Path.empty() && MovementCooldown <= diff)
    {
        pOrigin->Position = Path.top();
        Path.pop();
        MovementCooldown = 1000;
    }
    else
    {
        MovementCooldown -= diff;
    }
}

void Pathfinder::UpdateTarget(WorldObject* pNewTarget)
{
    this->pTarget = pNewTarget;
    this->Target = sf::Vector2i(pTarget->GetX(), pTarget->GetY());
    GeneratePath();
}

void Pathfinder::GeneratePath()
{
    while(!Path.empty())
    {
        Path.pop();
    }
    ResetPathfindingGrid();

    PathfinderNode** PathfindingGrid = pMap->PathfindingGrid;
    PathfinderNode* pOriginPathfinderNode = &PathfindingGrid[pOrigin->Position.y][pOrigin->Position.x];
    pOriginPathfinderNode->Status = OPEN;
    pOriginPathfinderNode->pParent = nullptr;
    pOriginPathfinderNode->Position = Target;

    std::priority_queue<PathfinderNode*> OpenList;
    OpenList.push(pOriginPathfinderNode);

    std::vector<std::vector<WorldObject*> >* pTileGrid = &pMap->TileGrid;

    do
    {
        PathfinderNode* pCurrent = OpenList.top();
        OpenList.pop();
        pCurrent->Status = CLOSED;

        if(pCurrent->Position == Target)
        {
            while(pCurrent->pParent != nullptr)
            {
                Path.push(std::move(pCurrent->Position));
                pCurrent = pCurrent->pParent;
            }
        }

        // Upper
        if((*pTileGrid)[pCurrent->Position.y-1][pCurrent->Position.x] == nullptr)
        {
            PathfinderNode* pAdjacent = &PathfindingGrid[pCurrent->Position.y-1][pCurrent->Position.x];
            if(CheckOrthogonalPathfinderNode(pCurrent, pAdjacent))
            {
                OpenList.push(pAdjacent);
            }
        }

        // Lower
        if((*pTileGrid)[pCurrent->Position.y+1][pCurrent->Position.x] == nullptr)
        {
            PathfinderNode* pAdjacent = &PathfindingGrid[pCurrent->Position.y-1][pCurrent->Position.x];
            if(CheckOrthogonalPathfinderNode(pCurrent, pAdjacent))
            {
                OpenList.push(pAdjacent);
            }
        }

        // Right
        if((*pTileGrid)[pCurrent->Position.y][pCurrent->Position.x+1] == nullptr)
        {
            PathfinderNode* pAdjacent = &PathfindingGrid[pCurrent->Position.y-1][pCurrent->Position.x];
            if(CheckOrthogonalPathfinderNode(pCurrent, pAdjacent))
            {
                OpenList.push(pAdjacent);
            }
        }

        // Left
        if((*pTileGrid)[pCurrent->Position.y-1][pCurrent->Position.x] == nullptr)
        {
            PathfinderNode* pAdjacent = &PathfindingGrid[pCurrent->Position.y-1][pCurrent->Position.x];
            if(CheckOrthogonalPathfinderNode(pCurrent, pAdjacent))
            {
                OpenList.push(pAdjacent);
            }
        }
    } while(!OpenList.empty());
}

bool Pathfinder::CheckOrthogonalPathfinderNode(PathfinderNode* pCurrent, PathfinderNode* pAdjacent)
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

void Pathfinder::ResetPathfindingGrid()
{
    std::memset(pMap->PathfindingGrid, UNKNOWN, 50 * 50 * sizeof(pMap->PathfindingGrid[0][0]));
}
