#include "pch.h"
#include "Global.h"
#include "TimeManager.h"

std::filesystem::path g_resourcePath;

RECT gBackgroundRect{ 150, 50, FRAME_BUFFER_WIDTH - 170, FRAME_BUFFER_HEIGHT - 70 };	// 이 수치를 조정해서 배경화면 그리기

CRITICAL_SECTION g_cs;

bool CheckTimer(int& currentTime, int targetTime)
{
	currentTime += GET_SINGLE(TimeManager)->GetDeltaTime();

	if (currentTime >= targetTime) {
		currentTime = 0.0f; // 타이머 초기화
		return true;         // 타이머 완료
	}
	return false;            // 아직 완료 안 됨
}