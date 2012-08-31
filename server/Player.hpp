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
#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Unit.hpp"
#include "WorldSession.hpp"
#include "Spell.hpp"
#include "Bag.hpp"

class WorldSession;

class WOD_DLL_DECL Player : public Unit
{
    friend class WorldSession;

public:
    Player(std::string Username, std::string Password, uint64 ObjID);
    ~Player();

    // TODO
    void AddToWorld();
    void RemoveFromWorld();

    void LoadFromDB();
    void SaveToDB();
    bool IsLoaded();

    bool CanSpeak() const;

    void SpellHit(SpellBoxPtr pSpellBox);

    void CastSpell(SpellPtr pSpell, float Angle);

    void Update(int64 diff);

    void SendPacket(WorldPacket* Packet);
    WorldPacket* PackData();
    void BindSession(WorldSession* pWorldSession);

    void LogOut();
    void Kick();
    bool IsInWorld();

    int GetSecLevel() const { return SecLevel;}
    std::string GetUsername() const { return Username; }

    bool HasItem(uint8 Bag, uint8 Slot, uint64 ItemID) const;
    bool IsValidPos(uint8 Bag, uint8 Slot) const;
    bool CanUnequipItem(uint8 Bag, uint8 Slot) const;
	bool CanEquip(uint8 Bag, uint8 Slot, uint8 DestSlot) const;

	uint8 FreeSlotBags() const;

    Item* GetItemByPos(uint8 SrcBag, uint8 SrcSlot) const;

    void AutoEquipItem(uint8 Bag, uint8 Slot);
    void EquipItem(uint8 SourceBag, uint8 SourceSlot, uint8 DestSlot);
    void EquipItem(Item* pItem, uint8 DestSlot);

    void EquipBag(Bag* pBag);

    void StoreItem(Item* pItem, uint8 DstBag, uint8 DstSlot);

    void UnequipItem(uint8 SrcSlot);
    void UnequipBag(uint8 SrcSlot);

    void UseItem(uint8 Bag, uint8 Slot);

    void SwapItem(uint8 SrcBag, uint8 SrcSlot, uint8 DstBag, uint8 DstSlot);

    void DestroyItem(uint8 SrcBag, uint8 SrcSlot);

private:
    void SendInventoryData();

    WorldSession* pWorldSession;

    std::string Username;
    std::string Password;

    int SecLevel;

    std::string Tileset;
    uint16 tx;
    uint16 ty;

    std::vector<uint16> Spells;

    bool LearnedSpell(uint8 ID);

    bool LoadedFromDB;
    bool m_IsMute;

    Bag* m_Bags[4];
};

#endif
