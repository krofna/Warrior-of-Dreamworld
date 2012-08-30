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
#include "Player.hpp"
#include "../shared/Opcodes.hpp"
#include "Database.hpp"
#include "World.hpp"

Player::Player(std::string Username, std::string Password, uint64 ObjID) :
Unit         (ObjID),
pWorldSession(nullptr),
Username     (Username),
Password     (Password),
LoadedFromDB (false),
m_IsMute     (false)
{
}

Player::~Player()
{
}

// This is bugged cause there may someone already be on the tile player wants to spawn on
void Player::AddToWorld()
{
    //pMap->TileGrid[Position.y][Position.x] = this;
}

// PH: needs more work
void Player::RemoveFromWorld()
{
    pMap->TileGrid[Position.y][Position.x].reset();
    PlayerPtr me = static_pointer_cast<Player>(shared_from_this());
    pMap->RemovePlayer(me);
}

void Player::LoadFromDB()
{
    QueryResult Result(sDatabase.PQuery("SELECT `seclevel`, `tileset`, `tx`, `ty`, `mapid`, `x`, `y` FROM `players` WHERE guid='%llu'", ObjID));
    Result->next();

    SecLevel    = Result->getUInt(1);
    Tileset     = Result->getString(2);
    tx          = Result->getUInt(3);
    ty          = Result->getUInt(4);
    pMap        = sWorld->GetMap(Result->getUInt(5));
    Position.x  = Result->getUInt(6);
    Position.y  = Result->getUInt(7);

    // Load bags
    QueryResult Result(sDatabase.PQuery("SELECT `guid`, `idx`, `item_id` FROM `character_bags` WHERE `owner_guid` = %llu", m_GUID));

    while (Result->next())
    {
        int idxBag = Result->getUInt(2);
        if (idxBag >= 4)
        {
            sLog.Write("Corrumpted database, invalid index bag !");
            break;
        }
        m_Bags[idxBag] = new Bag;
        m_Bags[idxBag]->LoadFromDB(Result->getUInt64(1), m_GUID, Result->getUInt64(3));
    }

    SendInventoryData();

    sDatabase.PExecute("UPDATE `players` SET `online` = 1 WHERE `guid` = %llu", ObjID);

    LoadedFromDB = true;
}

void Player::SendInventoryData()
{
    WorldPacket* Packet = new WorldPacket((uint16)MSG_INVENTORY_DATA);
    // TODO: What data are needed from client ?
    SendPacket(Packet);
}

bool Player::IsLoaded()
{
    return LoadedFromDB;
}

void Player::SpellHit(SpellBoxPtr pSpellBox)
{
    // PH, TODO: do player specific stuff
    Unit::SpellHit(pSpellBox);
}

void Player::CastSpell(SpellPtr pSpell, float Angle)
{
    // TODO: Uncomment this when player actually learns spells xD
    //if(!LearnedSpell(pSpell->ID))
        //return;

    Unit::CastSpell(pSpell, Angle);
}

bool Player::LearnedSpell(uint8 ID)
{
    for(auto SpellIter = Spells.begin(); SpellIter != Spells.end(); ++SpellIter)
    {
        if(*SpellIter == ID)
        {
            return true;
        }
    }
    return false;
}

void Player::Update(int32 diff)
{
}

void Player::SaveToDB()
{
    sDatabase.PExecute("UPDATE `players` SET x=%u, y=%u, mapid=%u WHERE guid=%llu", Position.x, Position.y, pMap->MapID, GetObjectID());
    // Save bags
    /*

    for (int i = 0 ; i < 4 ; ++i)
        if (m_Bags[i])
            m_Bags[i]->SaveToDB();

    */
}

WorldPacket* Player::PackData()
{
    WorldPacket* Packet = new WorldPacket((uint16)MSG_ADD_OBJECT);
    *Packet  << GetObjectID() << Tileset << Username << GetX() << GetY() << tx << ty;
    return Packet;
}

void Player::SendPacket(WorldPacket* Packet)
{
    pWorldSession->Send(Packet);
}

void Player::BindSession(WorldSession* pWorldSession)
{
    this->pWorldSession = pWorldSession;
}

bool Player::IsInWorld()
{
    return pWorldSession != nullptr;
}

void Player::Kick()
{
    LogOut();
}

void Player::LogOut()
{
    pWorldSession = nullptr;
    this->RemoveFromWorld();
    this->SaveToDB();
}

bool Player::CanSpeak() const
{
    return !m_IsMute;
}

bool Player::HasItem(uint8 Bag, uint8 Slot, uint64 ItemID) const
{
    if (!IsValidPos(Bag, Slot))
        return false;

    Item* pItem = m_Bags[Bag]->GetItemBySlot(Slot);
    
    return pItem->GetItemID() == ItemID;
}

bool Player::IsValidPos(uint8 Bag, uint8 Slot) const
{
    if (Bag >= 4)
        return false;

    if (!m_Bags[Bag]->IsValidSlot(Slot))
        return false;

    return true;
}

bool Player::CanUnequipItem(uint8 Bag, uint8 Slot) const
{
    Item* pItem = GetItemByPos(Bag, Slot);
    if (!pItem)
        return false;

    // TODO: What are conditions for an item is unequipable ? (Combat ?)

    return true;
}

Item* Player::GetItemByPos(uint8 Bag, uint8 Slot) const
{
    if (!IsValidPos(Bag, Slot))
        return nullptr;

    return m_Bags[Bag]->GetItemBySlot(Slot);
}

void Player::AutoEquipItem(uint8 Bag, uint8 Slot)
{
    if (!IsValidPos(Bag, Slot))
        return;

    Item* pItem = GetItemByPos(Bag, Slot);

    // TODO: Find the best slot for the item

    if (pItem->IsBag())
    {
        Bag* pBag = pItem->ToBag();
        if (FreeSlotBags() == 0)
        {
            pWorldSession->SendNotification("You haven't free slots for equip a new bag !");
            return;
        }
        else
        {
            EquipBag(pBag);
        }
    }
    else
    {

    }

    // EquipItem(pItem, bestDstSlot);
}

void Player::EquipBag(Bag* pBag)
{
    if (FreeSlotBags() == 0)
        return;

    for (int i = 0 ; i < 4 ; ++i)
    {
        if (!m_Bags[i])
        {
            m_Bags[i] = pBag;
            return;
        }
    }
}

void Player::EquipItem(uint8 SrcBag, uint8 SrcSlot, uint8 DstSlot)
{
    Item* pItem = GetItemByPos(SrcBag, SrcSlot);
    if (!pItem)
        return;

    EquipItem(pItem, DstSlot);
}

void Player::EquipItem(Item* pItem, uint8 DstSlot)
{
    // TODO: Equip item
}

void Player::UnequipItem(uint8 SrcSlot)
{
    // TODO: Unequip item
}

void Player::UnequipBag(uint8 SrcBag)
{
    if (!m_Bags[SrcBag])
        return;

    if (FreeSlotBags() == 3)
    {
        pWorldSession->SendNotification("You can't unequip your last bag !");
        return;
    }

    Bag* pBag = m_Bags[SrcBag];
    m_Bags[SrcBag] = nullptr;

    uint8 BagSlot = FindFreeBag();
    uint8 Slot = m_Bags[BagSlot]->FindFreeSlot();

    StoreItem(pBag, BagSlot, Slot);
}

void Player::StoreItem(Item* pItem, uint8 DstBag, uint8 DstSlot)
{
    if (!pItem || !IsValidPos(DstBag, DstSlot))
        return;

    m_Bags[DstBag]->Store(pItem, DstSlot);
}

void Player::UseItem(uint8 Bag, uint8 Slot)
{
    if (!IsValidPos(Bag, Slot))
        return;
}

void Player::SwapItem(uint8 SrcBag, uint8 SrcSlot, uint8 DstBag, uint8 DstSlot)
{
    if (!IsValidPos(SrcBag, SrcSlot) || !IsValidPos(DstBag, DstSlot))
        return;

    Item* pSrcItem = GetItemByPos(SrcBag, SrcSlot);
    Item* pDstItem = GetItemByPos(DstBag, DstSlot);

    std::swap(&pSrcItem, &pDstItem); // [To check]: Should work 
}
 
void Player::DestroyItem(uint8 SrcBag, uint8 SrcSlot)
{
    if (!IsValidPos(SrcBag, SrcSlot))
        return;
    
    m_Bags[SrcBag]->Destroy(SrcSlot);
}
