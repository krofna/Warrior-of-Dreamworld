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
#include <queue>
#include "../shared/Defines.hpp"
#include "../shared/Vector2.hpp"
#include <boost/shared_array.hpp>

/*
TODO: Instead of *50 use map width: see Map.cpp Map::{ctor}
*/

struct PathfinderNode
{
    PathfinderNode* pParent;
    Vector2i Position;
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
        // Called once to init static members
        static void Init();

        Pathfinder(WorldObjectPtr pOrigin);

        void Update(int32 diff);
        void UpdateTarget(WorldObjectPtr pNewTarget);

    private:
        // Status of tile used by this Pathfinder
        // Stored in pMap->PathfindingStatusGrid;
        enum TileStatus
        {
            UNKNOWN = 0,
            OPEN    = 1,
            CLOSED  = 2
        };

        // Generates path for pOrigin. Called when:
        // pOrigin finds new target or pTarget moved
        void GeneratePath();

        // Called by GeneratePath. Checks node adjacent to pCurrent
        // Node to be checked is at coordinates pCurrent->Position.x+x, ...y+y
        // Cost is 10 for orthogonal, 14 for diagonal nodes
        void CheckNode(PathfinderNode* pCurrent, int x, int y, int Cost);

        // WorldObject controlled by this pathfinder
        WorldObjectPtr pOrigin;

        // WorldObject chased by pOrigin
        WorldObjectPtr pTarget;

        // Position of pTarget when path was last generated
        // If it is different from pTarget->Position,
        // pTarget moved and path must be re-generated
        Vector2i Target;

        // Path generated by GeneratePath();
        // It is the path which pOrigin will take to reach pTarget
        std::stack<Vector2i> Path;

        // Pointer to map on which pOrigin is
        MapPtr pMap;

        // Pointer to 2D array of static world objects in pMap
        std::vector<std::vector<WorldObjectPtr> >* pTileGrid;

        // Pointer to 2D array of pre-allocated nodes used by pathfinder
        // Its size same as size of largest map
        // [TODO]: Maybe make it a bit bigger to get nicer path?
        static boost::shared_array<PathfinderNode> PathfindingGrid; // 2D (but 1D in real)
        static boost::shared_array<uint8> PathfindingStatusGrid;    // 2D (but 1D in real)

        // Time between moving onto next tile in path in miliseconds
        int32 MovementCooldown;

        // Priority queue of nodes to be checked by CheckNode()
        std::priority_queue<PathfinderNode*, std::vector<PathfinderNode*>, CompareNode> OpenList;

        //std::stack<Vector2i> PathToHome; // NYI - maybe calculate path to home but store only home coord in ctor
};

#endif
