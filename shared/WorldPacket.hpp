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
#ifndef WORLD_PACKET_H
#define WORLD_PACKET_H

#include "../shared/Defines.hpp"
#include <cassert>
#include <vector>

class WorldPacket
{
public:
    WorldPacket(size_t size);

    void Clear();

    char* GetData();
    uint16 GetOpcode();
    void SetOpcode(uint16 Opcode);
    uint16 GetSize();

    inline WorldPacket& operator <<(uint8 data);
    inline WorldPacket& operator <<(uint16 data);
    inline WorldPacket& operator <<(uint32 data);
    inline WorldPacket& operator <<(uint64 data);
    inline WorldPacket& operator <<(int8 data);
    inline WorldPacket& operator <<(int16 data);
    inline WorldPacket& operator <<(int32 data);
    inline WorldPacket& operator <<(int64 data);
    inline WorldPacket& operator <<(float data);
    inline WorldPacket& operator <<(std::string data);

    inline WorldPacket& operator >>(uint8& data);
    inline WorldPacket& operator >>(uint16& data);
    inline WorldPacket& operator >>(uint32& data);
    inline WorldPacket& operator >>(uint64& data);
    inline WorldPacket& operator >>(int8& data);
    inline WorldPacket& operator >>(int16& data);
    inline WorldPacket& operator >>(int32& data);
    inline WorldPacket& operator >>(int64& data);
    inline WorldPacket& operator >>(float& data);
    inline WorldPacket& operator >>(std::string& data);

    enum
    {
        HEADER_SIZE = sizeof(uint16) + sizeof(uint16)
    };


private:
    template<class T> void Append(T data);
    template<class T> T Read() const;

    uint16 Opcode;
    size_t ReadPos, WritePos;

    std::vector<char> ByteBuffer;
};

template<class T> void WorldPacket::Append(T data)
{
    // TODO: fixed sized packets?
    //ByteBuffer.resize(ByteBuffer.size() + sizeof(data));
    std::memcpy((void*)&ByteBuffer[WritePos], &data,  sizeof(data));
    WritePos += sizeof(data);
}

template<class T> T WorldPacket::Read() const
{
    assert(ReadPos + sizeof(T) <= ByteBuffer.size());
    T data = *((T*)&ByteBuffer[ReadPos]);
    ReadPos += sizeof(T);
    return data;
}

#endif