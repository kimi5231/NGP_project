#pragma once
struct Header
{
    PacketID id;
    int dataSize;
};

enum PacketID
{
    C_UpdateObjectState,
    C_UpdateDir,
    C_Move,
    C_Collision,
    C_UseItem,
    C_StayGame,
    C_EndGame,
};