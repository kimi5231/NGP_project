#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include <memory>
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <utility>

#include <tchar.h> // _T(), ...
#include <stdio.h> // printf(), ...
#include <stdlib.h> // exit(), ...
#include <string.h> // strncpy(), ...

#pragma comment(lib, "ws2_32")

#define FRAME_BUFFER_WIDTH 800
#define FRAME_BUFFER_HEIGHT 600
#define CELL_SIZE 30

// 소켓 함수 오류 출력 후 종료
void err_quit(const char* msg);
// 소켓 함수 오류 출력
void err_display(const char* msg);
// 소켓 함수 오류 출력
void err_display(int errcode);

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
    Bomb,
    UI,
    Obstacle,
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

    float Length() const {
        return std::sqrt(x * x + y * y);
    }

    void Normalize() {
        float len = Length();
        if (len == 0) {
            x = 0;
            y = 0;
            return;
        }
        x /= len;
        y /= len;
    }
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

// Server Packet
struct S_AddObject_Packet
{
	int objectID;
	ObjectType type;
	Vertex pos;
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

// Sound
enum class SoundType
{
    BGM,
    Normal,
};