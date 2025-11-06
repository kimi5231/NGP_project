#include "pch.h"
#include "TimeManager.h"

void TimeManager::Init()
{
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_frequency));
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_prevCount)); // CPU 클럭
	_timeScale = 1.0 / (double)_frequency;
}

void TimeManager::Update()
{
	unsigned __int64 currentCount;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

	// 경과 시간(초) = 현재 시간 - 전 시간 / 빈도
	_deltaTime = (currentCount - _prevCount) / static_cast<float>(_frequency);
	_prevCount = currentCount;

	_frameCount++;	// 몇 번 호출 됐는지 계속 추적
	_frameTime += _deltaTime;	// 경과된 시간 추적

	if (_frameTime >= 1.f)
	{
		// 1초동안 몇 번의 함수가 호출됐는지, fps 구하는 게 핵심
		_fps = static_cast<unsigned __int32>(_frameCount / _frameTime);

		_frameTime = 0.f;
		_frameCount = 0;
	}
}

void TimeManager::Tick(float lockFps)
{
	unsigned long long currentCount;
	QueryPerformanceCounter((LARGE_INTEGER*)&currentCount);

	float timeElapse = float((currentCount - _prevCount) * _timeScale);

	if (lockFps > 0.0f)
	{
		// 원하는 delta time이 아니면 loop
		while (timeElapse < (1.0f / lockFps))
		{
			QueryPerformanceCounter((LARGE_INTEGER*)&currentCount);

			timeElapse = float((currentCount - _prevCount) * _timeScale);
		}
	}

	_prevCount = currentCount;

	// 마지막 프레임 처리 시간과 현재 프레임 처리 시간의 차이가 1초보다 작으면 현재 프레임 처리 시간을 frame_time[0]에 저장
	if (fabsf(timeElapse - _deltaTime) < 1.0f)
	{
		::memmove(&_frameTimeSample[1], _frameTimeSample, (_maxSampleCount- 1) * sizeof(float));
		_frameTimeSample[0] = timeElapse;
		if (_sampleCount < _maxSampleCount) ++_sampleCount;
	}

	// 프레임++, 현재 프레임 처리 시간을 누적 하고 저장
	++_fps;
	_frameTime += timeElapse;
	if (_frameTime> 1.0f) {
		_currentFps = _fps;
		_fps = 0;
		_frameTime = 0.0f;
	}

	// 샘플링
	timeElapse = 0.0f;
	for (ULONG i = 0; i < _sampleCount; i++) _deltaTime += _frameTimeSample[i];
	if (_sampleCount > 0) _deltaTime /= _sampleCount;
}