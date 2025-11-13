#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include <memory>
#include <iostream>
#include <vector>
#include <functional>

#pragma comment(lib, "ws2_32")

// 임시적으로 정의
#define FRAME_BUFFER_WIDTH 800
#define FRAME_BUFFER_HEIGHT 600
#define CELL_SIZE 30

enum class ObjectState
{
    Idle,
    Move,
    UseSkill,
    Dead,
};

enum class ObjectType
{
    Player,
    Monster,
    Button,
    Item,
    Bullet,
};

enum class ItemType
{
    Life,
    Magazine,
    Lightning,
    Waterwheel,
    Coffee,
    Shotgun,
    Hourglass,
};

enum class Dir
{
   Left,
   Right,
   Up,
   Down,

   LeftUp,
   LeftDown,
   RightUp,
   RightDown,
};

struct Vertex
{
    int x, y;
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

    S_AddObject,
};

struct Header
{
    PacketID id;
    int dataSize;
};

// Client Packet
struct C_Move_Packet
{
    int objectID;
    ObjectType type;
    Vertex pos;
};

// Server Packet
struct C_AddObject_Packet
{
    int objectID;
    ObjectType type;
    Vertex pos;
};

using GameObjectRef = std::shared_ptr<class GameObject>;
using PlayerRef = std::shared_ptr<class Player>;
using ItemRef = std::shared_ptr<class Item>;
using ProjectileRef = std::shared_ptr<class Projectile>;

#define DECLARE_SINGLE(classname)           \
private:                                    \
    classname() { }                         \
public:                                     \
    static classname* GetInstance()         \
    {                                       \
        static classname s_instance;        \
        return &s_instance;                 \
    }

#define GET_SINGLE(classname)    classname::GetInstance()