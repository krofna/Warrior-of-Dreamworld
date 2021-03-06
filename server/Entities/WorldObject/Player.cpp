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
#include "CommandHandler.hpp"
#include "shared/Opcodes.hpp"
#include "shared/Log.hpp"
#include "shared/Math.hpp"
#include "Database.hpp"
#include "Map.hpp"
#include "World.hpp"

Player::Player(std::string Username, std::string Password, uint64 ObjID) :
Unit          (ObjID),
Username      (Username),
Password      (Password),
LoadedFromDB  (false),
m_IsMute      (false),
LastDirection (MOVE_STOP)
{
    for (int i = 0 ; i < 4 ; ++i)
        m_Bags[i] = nullptr;
}

Player::~Player()
{
}

void Player::LoadFromDB()
{
    QueryResult Result(sDatabase.PQuery("SELECT `seclevel`, `tileset`, `tx`, `ty`, `mapid`, `x`, `y` FROM `players` WHERE guid='%llu'", ObjID));
    Result->next();

    SecLevel    = Result->getUInt   (1);
    Tileset     = Result->getString (2);
    tx          = Result->getUInt   (3);
    ty          = Result->getUInt   (4);
    pMap        = sWorld->GetMap    (Result->getUInt(5));
    Position.x  = Result->getUInt   (6);
    Position.y  = Result->getUInt   (7);

    // Load bags
    QueryResult BagResult(sDatabase.PQuery("SELECT `guid`, `idx`, `item_id` FROM `character_bags` WHERE `owner_guid` = %llu", ObjID));

    while (BagResult->next())
    {
        int idxBag = Result->getUInt(2);
        if (idxBag >= 4)
        {
            sLog.Write("Corrupted database, invalid index bag !");
            break;
        }
        m_Bags[idxBag] = new Bag;
        m_Bags[idxBag]->LoadFromDB(Result->getUInt64(1), ObjID, Result->getUInt64(3));
    }

    sDatabase.PExecute("UPDATE `players` SET `online` = 1 WHERE `guid` = %llu", ObjID);

    LoadedFromDB = true;
}

void Player::SendInventoryData()
{
    WorldPacket Packet((uint16)MSG_INVENTORY_DATA);

    // Writing bags data
    for (int i = 0 ; i < 4 ; ++i)
    {
        if (m_Bags[i])
        {
            Packet << (uint8)Bag::BAG_USED;
            // Bag data
            m_Bags[i]->BuildPacketData(Packet);
        }
        else
            Packet << (uint8)Bag::BAG_UNUSED;
    }

    SendPacket(Packet);
}

bool Player::IsLoaded()
{
    return LoadedFromDB;
}

void Player::SpellHit(SpellBox* pSpellBox)
{
    Unit::SpellHit(pSpellBox);
}

void Player::CastSpell(SpellTemplate* pSpell, float Angle)
{
    Unit::CastSpell(pSpell, Angle);
}

void Player::Update(int64 diff)
{

}

void Player::SaveToDB()
{
    sDatabase.PExecute("UPDATE `players` SET x=%u, y=%u, mapid=%u WHERE guid=%llu", Position.x, Position.y, pMap->GetID(), GetObjectID());
    // Save bags
    /*

    for (int i = 0 ; i < 4 ; ++i)
        if (m_Bags[i])
            m_Bags[i]->SaveToDB();

    */
}

WorldPacket Player::PackData()
{
    WorldPacket Packet((uint16)MSG_ADD_OBJECT);
    Packet  << GetObjectID() << Tileset << Username << GetX() << GetY() << tx << ty;
    return Packet;
}

void Player::SendPacket(WorldPacket& Packet)
{
    pWorldSession->Send(Packet);
}

void Player::BindSession(WorldSessionPtr pWorldSession)
{
    this->pWorldSession = pWorldSession;
}

bool Player::IsInWorld() const
{
    return pWorldSession != nullptr;
}

void Player::OnServerShutdown()
{
    WorldPacket Packet((uint16)MSG_LOG_OUT);
    SendPacket(Packet);
    pWorldSession = WorldSessionPtr();
    this->SaveToDB();
}

void Player::LogOut()
{
    pWorldSession = WorldSessionPtr();
    pMap->RemovePlayer(this);
    this->SaveToDB();
}

bool Player::UpdateCoordinates(uint8 Direction)
{
    LastDirection = Direction;
    if (pMap->CheckOutside(Position.x, Position.y, Direction))
        return false;
    
    Vector2i OldPos = Position;
    bool Retval = true;

    switch(Direction)
    {
    case MOVE_UP:
        if(pMap->TryInteract(this, Position.x, Position.y-1))
            Retval = false;
        else
            Position.y--;
        break;
    case MOVE_DOWN:
        if(pMap->TryInteract(this, Position.x, Position.y+1))
            Retval = false;
        else
            Position.y++;
        break;
    case MOVE_LEFT:
        if(pMap->TryInteract(this, Position.x-1, Position.y))
            Retval = false;
        else
            Position.x--;
        break;
    case MOVE_RIGHT:
        if(pMap->TryInteract(this, Position.x+1, Position.y))
            Retval = false;
        else
            Position.x++;
        break;
    }
    
    if (Retval)
    {
        pMap->RemoveFromTileGrid(OldPos.x, OldPos.y);
        pMap->AddToTileGrid(this);
    }

    return Retval;
}

bool Player::CanSpeak() const
{
    return !m_IsMute;
}

bool Player::CanAttack(Unit* pTarget) const
{
    // Check distance between pTarget and Player
    if (math::GetManhattanDistance(GetPosition(), pTarget->GetPosition()) > 1.f)
        return false;

    return true;
}

Unit* Player::FindNearTarget(uint8 Direction) const
{
    Vector2i newPos = GetPosition();
    
    switch (Direction)
    {
        case MOVE_STOP:
        default:
            break;
        case MOVE_UP:
            newPos.y--;
            break;
        case MOVE_DOWN:
            newPos.y++;
            break;
        case MOVE_LEFT:
            newPos.x--;
            break;
        case MOVE_RIGHT:
            newPos.x++;
            break;
    }

    if (!pMap->GetObjectAt(newPos.x, newPos.y))
    {
        // Maybe a player ?
        sLog.Write("TODO: Find near player !");
        return nullptr;
    }

    return dynamic_cast<Unit*>(pMap->GetObjectAt(newPos.x, newPos.y));
}

Unit* Player::FindNearTarget() const
{
    return FindNearTarget(LastDirection);
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

bool Player::CanEquip(uint8 Bag, uint8 Slot, uint8 DestSlot) const
{
	Item* pItem = GetItemByPos(Bag, Slot);

	if (!pItem)
		return false;

	// TODO: Check if it's a weapon, and the dest slot is reserved for weapons, blah blah

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

uint8 Player::FreeSlotBags() const
{
	uint8 Accumulator = 0;
	for (int iBag = 0 ; iBag < 4 ; ++iBag)
	{
		if (m_Bags[iBag])
			Accumulator += m_Bags[iBag]->NumberFreeSlots();
	}

	return Accumulator;
}

Item* Player::GetItemByPos(uint8 Bag, uint8 Slot) const
{
    if (!IsValidPos(Bag, Slot))
        return nullptr;

    return m_Bags[Bag]->GetItemBySlot(Slot);
}

void Player::AutoEquipItem(uint8 SrcBag, uint8 Slot)
{
    if (!IsValidPos(SrcBag, Slot))
        return;

    Item* pItem = GetItemByPos(SrcBag, Slot);

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

    uint8 BagSlot, Slot;
	for (uint8 iBag = 0 ; iBag < 4 ; ++iBag)
	{
		if (!m_Bags[iBag])
			continue;

		BagSlot = iBag;
		Slot = m_Bags[BagSlot]->FindFreeSlot();
		if (Slot != BAG_FULL_SLOT)
			break;
	}

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

    Item* cpSrcItem(new Item(*pSrcItem)); // Verify copy operator !

    delete pSrcItem;

    StoreItem(pDstItem, SrcBag, SrcSlot);
    StoreItem(cpSrcItem, DstBag, DstSlot);
}
 
void Player::DestroyItem(uint8 SrcBag, uint8 SrcSlot)
{
    if (!IsValidPos(SrcBag, SrcSlot))
        return;
    
    m_Bags[SrcBag]->Destroy(SrcSlot);
}

std::string Player::GetEmote(const char* Text)
{
    // TODO: Handle also others emotes
    
    std::string EmoteText = Text;

    EmoteText.erase(EmoteText.begin(), EmoteText.begin() + 3); // Erase /me 

    return EmoteText;
}

bool Player::IsNotEmote(const char* Text)
{
    return (Text[0] != '/');
}

void Player::Say(const char* Text)
{
    OnChat(Text);

    if (IsNotEmote(Text) && Text[0] != '.') // Not a command or a emote
    {
        Unit::Say(Text);
    }
    else if (Text[0] != '.') // Not a command so a emote
    {
        WorldPacket TextEmotePacket((uint16)MSG_TEXT_EMOTE);
        std::string TextEmote = GetEmote(Text);
        TextEmotePacket << GetObjectID();
        TextEmotePacket << TextEmote;

        pMap->SendToPlayers(TextEmotePacket);
    }
}

void Player::OnChat(const char* Text)
{
    if (Text[0] == '.')
    {
        std::string Msg(Text);
        Msg.erase(Msg.begin());

        try
        {
            CommandHandler handler(this, Msg);
            if (!handler.ExecuteCommand())
            {
                SendNotification("Unknown command !");
            }
        }
        catch(CommandHandler::BadCommand const& /*e*/)
        {
            SendNotification("Unknown command !");
        }
    }
}

void Player::SendCommandReponse(std::string const& Msg)
{
    WorldPacket ReponsePacket((uint16)MSG_COMMAND_REPONSE);
    ReponsePacket << Msg;
    SendPacket(ReponsePacket);
}

void Player::SendNotification(std::string const& Msg)
{
    WorldPacket NotificationPacket((uint16)MSG_NOTIFICATION_MSG);
    NotificationPacket << Msg;
    SendPacket(NotificationPacket);
}
