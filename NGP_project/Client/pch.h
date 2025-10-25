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

};

enum class ObjectType
{
    Player,
    Monster,
    Background,
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

using PlayerRef = std::shared_ptr<class Player>;

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