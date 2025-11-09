#pragma once
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

struct Header
{
    PacketID id;
    int dataSize;
};