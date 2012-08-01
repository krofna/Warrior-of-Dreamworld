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
#include "WorldPacket.hpp"
#include "Opcodes.hpp"

WorldPacket::WorldPacket(size_t size)
{
    ByteBuffer.reserve(size);
}

void WorldPacket::Clear()
{
    ByteBuffer.clear();
    ReadPos = WritePos = 4;
    Opcode = (uint16)MSG_NULL;
}

size_t WorldPacket::GetSize()
{
    return ByteBuffer.size();
}

char* WorldPacket::GetData()
{
    return (char*)&ByteBuffer[0];
}

uint16 WorldPacket::GetOpcode()
{
    return Opcode;
}

void WorldPacket::SetOpcode(uint16 Opcode)
{
    this->Opcode = Opcode;
}

WorldPacket& WorldPacket::operator >>(uint8 data)   { Append<uint8>(data);  return *this; }
WorldPacket& WorldPacket::operator >>(uint16 data)  { Append<uint16>(data); return *this; }
WorldPacket& WorldPacket::operator >>(uint32 data)  { Append<uint32>(data); return *this; }
WorldPacket& WorldPacket::operator >>(uint64 data)  { Append<uint64>(data); return *this; }
WorldPacket& WorldPacket::operator >>(int8 data)    { Append<int8>(data);   return *this; }
WorldPacket& WorldPacket::operator >>(int16 data)   { Append<int16>(data);  return *this; }
WorldPacket& WorldPacket::operator >>(int32 data)   { Append<int32>(data);  return *this; }
WorldPacket& WorldPacket::operator >>(int64 data)   { Append<int64>(data);  return *this; }

WorldPacket& WorldPacket::operator <<(uint8& data)  { data = Read<int8>();  return *this; }
WorldPacket& WorldPacket::operator <<(uint16& data) { data = Read<int16>(); return *this; }
WorldPacket& WorldPacket::operator <<(uint32& data) { data = Read<uint32>();return *this; }
WorldPacket& WorldPacket::operator <<(uint64& data) { data = Read<uint64>();return *this; }
WorldPacket& WorldPacket::operator <<(int8& data)   { data = Read<int8>();  return *this; }
WorldPacket& WorldPacket::operator <<(int16& data)  { data = Read<int16>(); return *this; }
WorldPacket& WorldPacket::operator <<(int32& data)  { data = Read<int32>(); return *this; }
WorldPacket& WorldPacket::operator <<(int64& data)  { data = Read<int64>(); return *this; }