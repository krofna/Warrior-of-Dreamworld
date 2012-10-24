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
#include "WorldSession.hpp"
#include "shared/Opcodes.hpp"
#include "Map.hpp"
#include "Player.hpp"
#include <boost/bind.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/placeholders.hpp>
#include "shared/Log.hpp"

WorldSession::WorldSession(boost::asio::io_service& io) :
Socket                    (io),
pPlayer                   (nullptr)
{
}

WorldSession::~WorldSession()
{
}

Player* WorldSession::GetPlayer()
{
    return pPlayer;
}

void WorldSession::Start()
{
    Packet = WorldPacket((uint16)MSG_NULL);

    boost::asio::async_read(Socket,
        boost::asio::buffer(Packet.GetDataWithHeader(), WorldPacket::HEADER_SIZE),
        boost::bind(&WorldSession::HandleHeader, shared_from_this(), boost::asio::placeholders::error));
}

void WorldSession::HandleHeader(const boost::system::error_code& Error)
{
    Packet.ReadHeader();

    // Header only packet
    if(Packet.GetSizeWithoutHeader() < 1 || Error)
    {
        HandleReceive(Error);
        return;
    }

    boost::asio::async_read(Socket,
        boost::asio::buffer(Packet.GetDataWithoutHeader(), Packet.GetSizeWithoutHeader()),
        boost::bind(&WorldSession::HandleReceive, shared_from_this(), boost::asio::placeholders::error));
}

void WorldSession::HandleReceive(const boost::system::error_code& Error)
{
    Packet.ResetReadPos();
    if(Error)
    {
        sLog.Write("Failed to receive packet. Kicking player. TODO: Try Reconnect");
        if (pPlayer)
            pPlayer->LogOut();
        return;
    }
    if(Packet.GetOpcode() >= MSG_COUNT)
    {
        sLog.Write("Received %u: Bad opcode! Kicking player.", Packet.GetOpcode());
        if (pPlayer)
            pPlayer->LogOut();
        return;
    }
	if(Packet.GetOpcode() == MSG_NULL)
    {
        sLog.Write("Received a MSG_NULL, strange...");
    }
    sLog.Write("Received Packet: %s, ", OpcodeTable[Packet.GetOpcode()].name);

    (this->*OpcodeTable[Packet.GetOpcode()].Handler)();

    Start();
}

void WorldSession::Send(WorldPacket& Packet)
{
    sLog.Write("Sending Packet: %s, ", OpcodeTable[Packet.GetOpcode()].name);

    Packet.UpdateSizeData();
    MessageQueue.push(Packet);
    if(MessageQueue.size() == 1)
    {
        boost::asio::async_write(Socket,
            boost::asio::buffer(Packet.GetDataWithHeader(), Packet.GetSizeWithHeader()),
            boost::bind(&WorldSession::HandleSend, shared_from_this(), boost::asio::placeholders::error));
    }
}

void WorldSession::HandleSend(const boost::system::error_code& Error)
{
    if(!Error)
    {
        sLog.Write("Successful!");
    }
    else
    {
        sLog.Write("Failed!");
        // Return packet to queue
        // It will be sent after reconnect
    }

    MessageQueue.pop();
    if(!MessageQueue.empty())
    {
        boost::asio::async_write(Socket,
            boost::asio::buffer(MessageQueue.front().GetDataWithHeader(), MessageQueue.front().GetSizeWithHeader()),
            boost::bind(&WorldSession::HandleSend, shared_from_this(), boost::asio::placeholders::error));
    }
}

void WorldSession::SendLoginFailPacket(uint16 Reason)
{
    WorldPacket Packet((uint16)MSG_LOGIN);
    Packet << Reason;
    Send(Packet);
    Socket.close();
}

void WorldSession::SendChatMessage(uint64 FromID, std::string const& Message)
{
    WorldPacket Packet((uint16)MSG_CHAT_MESSAGE);
    Packet << FromID << Message;
    Send(Packet);
}

void WorldSession::HandleNULL()
{
}

void WorldSession::HandleLoginOpcode()
{
    // Check if username exists
    std::string Username;
    Packet >> Username;
    pPlayer = sObjectMgr.GetPlayer(Username);
    if(!pPlayer)
    {
        // Invalid username, send response
        SendLoginFailPacket((uint16)LOGIN_FAIL_BAD_USERNAME);
        return;
    }

    // Check if passwords match
    std::string Password;
    Packet >> Password;
    if(pPlayer->Password != Password)
    {
        // Invalid password, send response
        SendLoginFailPacket((uint16)LOGIN_FAIL_BAD_PASSWORD);
        return;
    }

    // If player is online, kick him
    if(pPlayer->IsInWorld())
        pPlayer->LogOut();
    else if(!pPlayer->IsLoaded())
        pPlayer->LoadFromDB();

    // Tell the client that he logged in sucessfully
    WorldPacket LoginPacket((uint16)MSG_LOGIN);
    LoginPacket << (uint16)LOGIN_SUCCESS
                << pPlayer->GetMapID()
                << pPlayer->GetObjectID();
    Send(LoginPacket);

    // Bind WorldSession to Player
    pPlayer->BindSession(shared_from_this());

    // Send inventory data to client
    pPlayer->SendInventoryData();

    // Add player to the map
    pPlayer->GetMap()->AddPlayer(pPlayer);
    sLog.Write("Packet is good!");
}

void WorldSession::HandleMoveObjectOpcode()
{
    uint8 Direction;
    Packet >> Direction;

    sLog.Write("Packet is good!");

    // If player colided, return
    if(!pPlayer->UpdateCoordinates(Direction))
        return;

    // Send movement update to all players in the map
    WorldPacket Packet((uint16)MSG_MOVE_OBJECT);
    Packet << pPlayer->GetObjectID() << pPlayer->GetX() << pPlayer->GetY();
    pPlayer->GetMap()->SendToPlayers(Packet);
}

void WorldSession::HandleCastSpellOpcode()
{
    uint16 SpellID;
    float Angle;
    Packet >> SpellID >> Angle;

    SpellTemplate* pSpell = sObjectMgr.GetSpellTemplate(SpellID);

    if(!pSpell)
    {
        sLog.Write("Invalid Spell ID!");
        SendNotification("Unknown Spell!");
        return;
    }

    sLog.Write("Packet is good!");

    pPlayer->CastSpell(pSpell, Angle);
}

void WorldSession::HandleMeleeAttackOpcode()
{
    Unit* pTarget = pPlayer->FindNearTarget();

    if (!pTarget)
    {
        pPlayer->SendNotification("No target !");
        return;
    }

    if (!pPlayer->CanAttack(pTarget))
    {
        pPlayer->SendNotification("Impossible to attack target !");
        return;
    }

    pPlayer->DealDamage(pPlayer->GetMeleeDamage(), pTarget);
}

void WorldSession::HandleLogOutOpcode()
{
    SendLogOutPacket();
    pPlayer->LogOut();
}

void WorldSession::HandleChatMessageOpcode()
{
    std::string Message;
    Packet >> Message;

    if (!pPlayer->CanSpeak())
    {
        SendNotification("You can't speak !");
        return;
    }

    pPlayer->Say(Message.c_str());
}

void WorldSession::HandleAutoEquipItemOpcode()
{
    uint64 ItemGUID;
    uint8 SourceBag, SourceSlot;
    Packet >> ItemGUID >> SourceBag >> SourceSlot;

    // Cheating attempt
    if (!pPlayer->HasItem(SourceBag, SourceSlot, ItemGUID))
    {
        SendNotification("Item not found!");
        return;
    }

    pPlayer->AutoEquipItem(SourceBag, SourceSlot);
}

void WorldSession::HandleUseItemOpcode()
{
    uint64 ItemGUID;
    uint8 SourceBag, SourceSlot;

    Packet >> ItemGUID >> SourceBag >> SourceSlot;

    // Cheating attempt
    if (!pPlayer->HasItem(SourceBag, SourceSlot, ItemGUID))
    {
        SendNotification("Item not found!");
        return;
    }

    pPlayer->UseItem(SourceBag, SourceSlot);
}

void WorldSession::HandleEquipItemOpcode()
{
    uint8 SourceSlot, SourceBag, DestEquipementSlot;
    uint64 ItemGUID;

    Packet >> ItemGUID >> SourceBag >> SourceSlot >> DestEquipementSlot;

    if (!pPlayer->HasItem(SourceBag, SourceSlot, ItemGUID))
    {
        SendNotification("Item not found!");
        return;
    }

    if (!pPlayer->CanEquip(SourceBag, SourceSlot, DestEquipementSlot))
    {
        SendNotification("Can't equip this item !");
        return;
    }

    pPlayer->EquipItem(SourceBag, SourceSlot, DestEquipementSlot);
}

void WorldSession::HandleSwapItemOpcode()
{
    uint8 SourceBag, SourceSlot, DestinationBag, DestinationSlot;
    Packet >> SourceBag >> DestinationBag >> SourceSlot >> DestinationSlot;

    if (SourceSlot == DestinationSlot && SourceBag == DestinationBag)
        return;

    if (!pPlayer->IsValidPos(SourceBag, SourceSlot))
    {
        SendNotification("Item not found!");
        return;
    }

    if (!pPlayer->IsValidPos(DestinationBag, DestinationSlot))
    {
        SendNotification("Item can't go in this slot !");
        return;
    }

    pPlayer->SwapItem(SourceBag, SourceSlot, DestinationBag, DestinationSlot);
}

void WorldSession::HandleDeleteItemOpcode()
{
    uint8 Bag, Slot, Count;
    Packet >> Bag >> Slot >> Count;

    if (!pPlayer->CanUnequipItem(Bag, Slot))
    {
        SendNotification("You can't currently destroy this item !");
        return;
    }

    Item* pItem = pPlayer->GetItemByPos(Bag, Slot);

    if (!pItem)
    {
        SendNotification("Item not found!");
        return;
    }

    pPlayer->DestroyItem(Bag, Slot);
}

void WorldSession::HandleBuyItemOpcode()
{
}

void WorldSession::HandleSellItemOpcode()
{
}

void WorldSession::HandleAcceptQuestOpcode()
{
}

void WorldSession::SendLogOutPacket()
{
    WorldPacket Packet((uint16)MSG_LOG_OUT);
    Send(Packet);
}

void WorldSession::SendNotification(std::string const& NotificationMessage)
{
    WorldPacket Packet((uint16)MSG_SYSTEM_MESSAGE);

    Packet << NotificationMessage;
    Send(Packet);
}
