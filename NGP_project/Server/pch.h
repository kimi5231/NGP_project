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

enum class CollisionType
{
    Monster,
    Item,
};

struct Vertex
{
    int x, y;
};

enum PacketID
{
    // Client
    C_UpdateObjectState,
    C_UpdateDir,
    C_Move,
    C_Collision,
    C_UseItem,
    C_StayGame,
    C_EndGame,

    // Server
    S_AddObject,
    S_RemoveObject,
    S_UpdateObjectState,
    S_UpdateDir,
    S_Move,
    S_ChangeNextStage,
    S_CollisionResult,
    S_MonsterDemaged,
    S_ItemUseResult,
};

struct Header
{
    PacketID id;
    int dataSize;
};

// Server Packet
struct S_AddObject_Packet
{
    int objectID;
    ObjectType type;
    Vertex pos;
};

struct S_RemoveObject_Packet
{
    int objectID;
    ObjectType type;
};

struct S_UpdateObjectState_Packet
{
    int objectID;
    ObjectType type;
    ObjectState state;
};

struct S_UpdateDir_Packet
{
    int objectID;
    ObjectType type;
    Dir dir;
};

struct S_Move_Packet
{
    int objectID;
    ObjectType type;
    Vertex pos;
};

struct S_ChangeNextStage_Packet
{
    int stageNum;
};

struct S_CollisionResult_Packet
{
    bool result;
};

struct S_MonsterDamaged_Packet
{
    int objectID;
    ObjectType type;
    int monsterHP;
};

struct S_ItemUseResult_Packet
{
    bool result;
};

// Client Packet
struct C_UpdateObjectState_Packet
{
    int objectID;
    ObjectType type;
    ObjectState state;
};

struct C_UpdateDir_Packet
{
    int objectID;
    ObjectType type;
    Dir dir;
};

struct C_Move_Packet
{
    int objectID;
    ObjectType type;
    Vertex pos;
};

struct C_Collision_Packet
{
    CollisionType collisionType;
    int objectID1;
    ObjectType type1;
    Vertex pos1;
    int objectID2;
    ObjectType type2;
    Vertex pos2;
};

struct C_UseItem_Packet
{
    int objectID;
    ObjectType itemType;
};

struct C_StayGame_Packet
{
    int objectID;
};

struct C_EndGame_Packet
{
    int objectID;
};

using ClientRef = std::shared_ptr<class Client>;
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