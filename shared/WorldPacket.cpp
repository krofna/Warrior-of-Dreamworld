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

WorldPacket::WorldPacket(uint16 Opcode) :
ByteBuffer              (2+2)
{
    std::memcpy((void*)&ByteBuffer[2], &Opcode, 2);
}

void WorldPacket::Clear()
{
    ByteBuffer.clear();
    ReadPos = WritePos = 0;
}

uint16 WorldPacket::GetSizeWithHeader()
{
    return ByteBuffer.size();
}

uint16 WorldPacket::GetSizeWithoutHeader()
{
    return ByteBuffer.size() - HEADER_SIZE;
}

void* WorldPacket::GetDataWithHeader()
{
    return &ByteBuffer[0];
}

void* WorldPacket::GetDataWithoutHeader()
{
    return &ByteBuffer[2];
}

uint16 WorldPacket::GetOpcode()
{
    return *(uint16*)&ByteBuffer[2];
}

void WorldPacket::SetOpcode(uint16 Opcode)
{
    std::memcpy((void*)&ByteBuffer[2], &Opcode, 2);
}

void WorldPacket::ReadHeader()
{
    ByteBuffer.resize(HEADER_SIZE + *(uint16*)&ByteBuffer[0]);
}

void WorldPacket::UpdateWritePos()
{
    WritePos = ByteBuffer.size();
}

void WorldPacket::ResetReadPos()
{
    ReadPos = HEADER_SIZE;
}

WorldPacket& WorldPacket::operator <<(uint8 data)   { Append<uint8>(data);  return *this; }
WorldPacket& WorldPacket::operator <<(uint16 data)  { Append<uint16>(data); return *this; }
WorldPacket& WorldPacket::operator <<(uint32 data)  { Append<uint32>(data); return *this; }
WorldPacket& WorldPacket::operator <<(uint64 data)  { Append<uint64>(data); return *this; }
WorldPacket& WorldPacket::operator <<(int8 data)    { Append<int8>(data);   return *this; }
WorldPacket& WorldPacket::operator <<(int16 data)   { Append<int16>(data);  return *this; }
WorldPacket& WorldPacket::operator <<(int32 data)   { Append<int32>(data);  return *this; }
WorldPacket& WorldPacket::operator <<(int64 data)   { Append<int64>(data);  return *this; }
WorldPacket& WorldPacket::operator <<(float data)   { Append<float>(data);  return *this; }
WorldPacket& WorldPacket::operator <<(std::string data)
{
    Append<uint16>(data.size());
    ByteBuffer.resize(ByteBuffer.size() + data.size());
    std::memcpy(&ByteBuffer[WritePos], data.c_str(), data.size());
    WritePos += data.size();
    return *this; 
}

WorldPacket& WorldPacket::operator >>(uint8& data)  { data = Read<uint8>(); return *this; }
WorldPacket& WorldPacket::operator >>(uint16& data) { data = Read<uint16>();return *this; }
WorldPacket& WorldPacket::operator >>(uint32& data) { data = Read<uint32>();return *this; }
WorldPacket& WorldPacket::operator >>(uint64& data) { data = Read<uint64>();return *this; }
WorldPacket& WorldPacket::operator >>(int8& data)   { data = Read<int8>();  return *this; }
WorldPacket& WorldPacket::operator >>(int16& data)  { data = Read<int16>(); return *this; }
WorldPacket& WorldPacket::operator >>(int32& data)  { data = Read<int32>(); return *this; }
WorldPacket& WorldPacket::operator >>(int64& data)  { data = Read<int64>(); return *this; }
WorldPacket& WorldPacket::operator >>(float& data)  { data = Read<float>(); return *this; }
WorldPacket& WorldPacket::operator >>(std::string& data)
{
    uint16 size = Read<uint16>();
    data.resize(size);
    std::memcpy(&data[0], &ByteBuffer[ReadPos], size);
    ReadPos += size;
    return *this; 
}
