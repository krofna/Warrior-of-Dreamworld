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
#include <boost/scoped_array.hpp>
#include "shared/Math.hpp"

class Map;
class WorldObject;
class Unit;

typedef std::vector<std::vector<WorldObject*> > WorldObjectGrid;

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

class my_priority_queue :
    public std::priority_queue<PathfinderNode*, std::vector<PathfinderNode*>, CompareNode>
{
public:
    void re_insert(PathfinderNode* what)
    {
        this->c.erase(std::find(this->c.begin(), this->c.end(), what));
        std::make_heap(this->c.begin(), this->c.end(), CompareNode());
        this->push(what);
    }

    void clear()
    {
        this->c.clear();
    }
    
    void reserve(size_t elements)
    {
        this->c.reserve(elements);
    }
};

class Pathfinder
{
    public:
        // Called once to init static members
        static void Init();

        Pathfinder(Unit* pOrigin);

        void Update(int64 diff);
        void UpdateTarget(Unit* pNewTarget);

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

        // Unit controlled by this pathfinder
        Unit* pOrigin;

        // Unit chased by pOrigin
        Unit* pTarget;

        // Position of pTarget when path was last generated
        // If it is different from pTarget->Position,
        // pTarget moved and path must be re-generated
        Vector2i Target;

        // Original position of Unit controlled by this
        // pathfinder. Used to generate path back home
        Vector2i Home;

        // Path generated by GeneratePath();
        // It is the path which pOrigin will take to reach pTarget
        std::stack<Vector2i> Path;

        // Pointer to map on which pOrigin is
        Map* pMap;

        // Pointer to 2D array of static world objects in pMap
        WorldObjectGrid* pTileGrid;

        // Pointer to 2D array of pre-allocated nodes used by pathfinder
        // Its size same as size of largest map
        // [TODO]: Maybe make it a bit bigger to get nicer path?
        static boost::scoped_array<PathfinderNode> PathfindingGrid; // 2D (but 1D in real)
        static boost::scoped_array<uint8> PathfindingStatusGrid;    // 2D (but 1D in real)

        // Time between moving onto next tile in path in miliseconds
        int64 MovementCooldown;

        // Priority queue of nodes to be checked by CheckNode()
        static my_priority_queue OpenList;
};

#endif
