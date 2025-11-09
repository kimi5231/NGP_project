#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include <memory>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

#pragma comment(lib, "ws2_32")

#define FRAME_BUFFER_WIDTH 800
#define FRAME_BUFFER_HEIGHT 600
#define CELL_SIZE 30

enum class ObjectState
{
    Idle,
    Move,
    Boom,
    Dead,
};

enum class ObjectType
{
    Player,
    Monster,
    Button,
    Item,
    Bullet,
    Bomb,
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

enum class CollisionType
{
    Monster,
    Item,
};

struct Vertex
{
    int x, y;
};

using GameObjectRef = std::shared_ptr<class GameObject>;
using PlayerRef = std::shared_ptr<class Player>;
using MonsterRef = std::shared_ptr<class Monster>;
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

enum C_PacketID
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
    C_PacketID id;
    int dataSize;
};

// Packet
struct C_UpdateObjectState
{
    int objectID;
    ObjectType type;
    ObjectState state;
};

struct C_UpdateDir
{
    int objectID;
    ObjectType type;
    Dir dir;
};

struct C_Move
{
    int objectID;
    ObjectType type;
    Vertex pos;
};

struct C_Collision
{
    CollisionType collisionType;
    int objectID1;
    ObjectType type1;
    Vertex pos1;
    int objectID2;
    ObjectType type2;
    Vertex pos2;
};

struct C_UseItem
{
    int objectID;
    ObjectType itemType;
};

struct C_StayGame
{
    int objectID;
};

struct C_EndGame
{
    int objectID;
};
