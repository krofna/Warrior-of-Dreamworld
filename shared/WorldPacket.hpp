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

#include "shared/Defines.hpp"
#include <cassert>
#include <vector>
#include <cstring>

// Todo:
// - void Clear(uint16 Opcode);
// - Base class ByteBuffer; same shit, but no Opcode,
// will be useful for Loadable::Load

// Implements heterogeneous container
// Not necessarily used for sending
// over network. See: Loadable::Load
class WOD_DLL_DECL WorldPacket
{
public:
    WorldPacket();
    WorldPacket(uint16 Opcode);

    void* GetDataWithHeader();
    void* GetDataWithoutHeader();
    uint16 GetOpcode();
    void SetOpcode(uint16 Opcode);
    uint16 GetSizeWithHeader();
    uint16 GetSizeWithoutHeader();
    void ReadHeader();
    void UpdateSizeData();
    void UpdateWritePos();
    void ResetReadPos();
    bool EndOfPacket();
    // bool IsCorrupted() const { return CorruptionState; }

    template<class T> void ReadSkip();

    WorldPacket& operator <<(uint8 data);
    WorldPacket& operator <<(uint16 data);
    WorldPacket& operator <<(uint32 data);
    WorldPacket& operator <<(uint64 data);
    WorldPacket& operator <<(int8 data);
    WorldPacket& operator <<(int16 data);
    WorldPacket& operator <<(int32 data);
    WorldPacket& operator <<(int64 data);
    WorldPacket& operator <<(float data);
    WorldPacket& operator <<(std::string data);

    WorldPacket& operator >>(uint8& data);
    WorldPacket& operator >>(uint16& data);
    WorldPacket& operator >>(uint32& data);
    WorldPacket& operator >>(uint64& data);
    WorldPacket& operator >>(int8& data);
    WorldPacket& operator >>(int16& data);
    WorldPacket& operator >>(int32& data);
    WorldPacket& operator >>(int64& data);
    WorldPacket& operator >>(float& data);
    WorldPacket& operator >>(std::string& data);

    enum
    {
        HEADER_SIZE = 4
    };


private:
    template<class T> void Append(T data);
    template<class T> T Read();

    size_t ReadPos, WritePos;

    //|   0,1   |   2,3   | 4,5,6,7...
    //|   - -   |   - -   | - - - -...
    //|  Size   | Opcode  | Data   ...
    std::vector<char> ByteBuffer;

    // bool CorruptionState;
};

template<class T> void WorldPacket::ReadSkip()
{
    ReadPos += sizeof(T);
}

template<class T> void WorldPacket::Append(T data)
{
    ByteBuffer.resize(ByteBuffer.size() + sizeof(data));
    std::memcpy((void*)&ByteBuffer[WritePos], &data, sizeof(data));
    WritePos += sizeof(data);
}

template<class T> T WorldPacket::Read()
{
    // [TODO]
    // For now we will assume that assert failed
    // because I am shitty programmer.
    // Packet should be dropped instead of crashing
    // server
    // [/TODO]
    assert(ReadPos + sizeof(T) <= ByteBuffer.size()); // [Comment by FullMetal-Alchimist: Why not make something like: CorruptionState = true; and make a test, if it is, drop the packet]
    // [??]
    // CorruptionState = true;
    // [/??]
    T data = *((T*)&ByteBuffer[ReadPos]);
    ReadPos += sizeof(T);
    return data;
}

#endif
