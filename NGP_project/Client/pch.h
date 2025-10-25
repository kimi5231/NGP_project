#pragma once

#include <windows.h>

#include <memory>
#include <iostream>
#include <vector>

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
    Background,
    Item,
};

enum class ItemType : int
{
    Life,
    Magazine,
    Lightning,
    Waterwheel,
    Coffee,
    Shotgun,
    Hourglass,
};

struct Vertex
{
    int x, y;
};

using GameObjectRef = std::shared_ptr<class GameObject>;
using PlayerRef = std::shared_ptr<class Player>;
using TankMonsterRef = std::shared_ptr<class TankMonster>;
using ItemRef = std::shared_ptr<class Item>;

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