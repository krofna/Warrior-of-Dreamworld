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
pMap                  (pOrigin->GetMap()),
pTarget               (nullptr)
{
    MovementCooldown = 1000;
}

void Pathfinder::Update(Int32 diff)
{
    if(pTarget && pTarget->Position != Target)
    {
        this->Target = sf::Vector2i(pTarget->GetX(), pTarget->GetY());
        GeneratePath();
    }
    if(!Path.empty() && (MovementCooldown <= diff))
    {
        printf("left: %u\n", Path.size());
        pOrigin->UpdatePosition(Path.top());
        Path.pop();
        MovementCooldown = 500;
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
    // Destroy old path if it exists
    while(!Path.empty())
    {
        Path.pop();
    }

    // Zero out status grid
    ResetPathfindingGrid();

    // Get pointer to 2D grid of pathfinding nodes in a map
    PathfinderNode* PathfindingGrid = pMap->PathfindingGrid;

    // Get Node with same position as creature in pathfinding grid
    PathfinderNode* pOriginNode = &PathfindingGrid[50 * pOrigin->GetY() + pOrigin->GetX()];

    // Origin node does not have parent node
    pOriginNode->pParent = nullptr;

    // Origin node does not have G cost
    pOriginNode->G = 0;
    pOriginNode->H = 10 * math::GetManhattanDistance(sf::Vector2i(pOriginNode->Position.x, pOriginNode->Position.y), Target);

    // Priority queue of nodes : Lower cost, higher priority
    std::priority_queue<PathfinderNode*, std::vector<PathfinderNode*>, CompareNode> OpenList;

    // Add origin node to queue
    OpenList.push(pOriginNode);

    // Get a pointer to WorldObject grid to check for collision
    std::vector<std::vector<WorldObject*> >* pTileGrid = &pMap->TileGrid;

    do
    {
        // Grab node with highest priority
        PathfinderNode* pCurrent = OpenList.top();
        OpenList.pop();

        // Move it to "closed" list
        pMap->PathfindingStatusGrid[50 * pCurrent->Position.y + pCurrent->Position.x] = CLOSED;

        // If current node has same coordinates as target, we found our path
        if(pCurrent->Position == Target)
        {
            // Create path using parents to track down origin
            while(pCurrent->pParent != nullptr)
            {
                Path.push(pCurrent->Position);
                pCurrent = pCurrent->pParent;
            }
            return;
        }

        // For each of adjacent nodes
        // if it is not out of bounds
        // and if tile is 'walkable', 
        // check its status

        // Upper
        if(pCurrent->Position.y != 0 && (*pTileGrid)[pCurrent->Position.y-1][pCurrent->Position.x] == nullptr)
        {
            PathfinderNode* pAdjacent = &PathfindingGrid[50 * (pCurrent->Position.y-1) + pCurrent->Position.x];
            if(CheckOrthogonalPathfinderNode(pCurrent, pAdjacent))
            {
                OpenList.push(pAdjacent);
            }
        }

        // Lower
        if(pCurrent->Position.y != 49 && (*pTileGrid)[pCurrent->Position.y+1][pCurrent->Position.x] == nullptr)
        {
            PathfinderNode* pAdjacent = &PathfindingGrid[50 * (pCurrent->Position.y+1) + pCurrent->Position.x];
            if(CheckOrthogonalPathfinderNode(pCurrent, pAdjacent))
            {
                OpenList.push(pAdjacent);
            }
        }

        // Right
        if(pCurrent->Position.x != 49 && (*pTileGrid)[pCurrent->Position.y][pCurrent->Position.x+1] == nullptr)
        {
            PathfinderNode* pAdjacent = &PathfindingGrid[50 * pCurrent->Position.y + pCurrent->Position.x+1];
            if(CheckOrthogonalPathfinderNode(pCurrent, pAdjacent))
            {
                OpenList.push(pAdjacent);
            }
        }

        // Left
        if(pCurrent->Position.x != 0 && (*pTileGrid)[pCurrent->Position.y][pCurrent->Position.x-1] == nullptr)
        {
            PathfinderNode* pAdjacent = &PathfindingGrid[50 * pCurrent->Position.y + pCurrent->Position.x-1];
            if(CheckOrthogonalPathfinderNode(pCurrent, pAdjacent))
            {
                OpenList.push(pAdjacent);
            }
        }
    } while(!OpenList.empty());

    printf("Could not generate path");
}

bool Pathfinder::CheckOrthogonalPathfinderNode(PathfinderNode* pCurrent, PathfinderNode* pAdjacent)
{
    // Check status
    switch(pMap->PathfindingStatusGrid[50 * pAdjacent->Position.y + pAdjacent->Position.x])
    {
        // If there is no status yet
    case (Uint8)UNKNOWN:
        // Make the current node parent
        pAdjacent->pParent = pCurrent;

        // Calculate costs
        pAdjacent->G = pCurrent->G + 10;
        pAdjacent->H = 10 * math::GetManhattanDistance(sf::Vector2i(pAdjacent->Position.x, pAdjacent->Position.y), Target);

        // Add it to open list
        pMap->PathfindingStatusGrid[50 * pAdjacent->Position.y + pAdjacent->Position.x] = OPEN;
        printf("adding new to open list: costs: %u, %u\n", pAdjacent->G, pAdjacent->H);
        return true;

        // If it is already on open list
    case (Uint8)OPEN:
        // Check if G path of pCurrent is better than G path of pAdjacent
        /*if((pCurrent->G + 10) < pAdjacent->G)
        {
            // Make pCurrent parent of pAdjacent
            pAdjacent->pParent = pCurrent;

            // Recalculate distance
            pAdjacent->G = pCurrent->G + 10;
            pAdjacent->H = 10 * math::GetManhattanDistance(sf::Vector2i(pAdjacent->Position.x, pAdjacent->Position.y), Target);

            // Do not add on open list
            return false;
        }*/

        // If its closed, ignore it
    case (Uint8)CLOSED:
        break;
    }

    return false;
}

void Pathfinder::ResetPathfindingGrid()
{
    std::memset(pMap->PathfindingStatusGrid, 0, 50 * 50 * sizeof(pMap->PathfindingStatusGrid[0]));
}
