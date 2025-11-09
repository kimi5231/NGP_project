#include "pch.h"
#include "Global.h"

std::filesystem::path g_resourcePath;

RECT gBackgroundRect{ 150, 50, FRAME_BUFFER_WIDTH - 150, FRAME_BUFFER_HEIGHT - 50 };	// 이 수치를 조정해서 배경화면 그리기