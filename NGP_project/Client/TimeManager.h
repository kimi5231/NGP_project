#pragma once
// 프레임과 시간 관련된 모든 것 관리

class TimeManager
{
	DECLARE_SINGLE(TimeManager);

public:
	void Init();
	void Update();
	void Tick(float lockFps);

	unsigned __int32 GetFps() { return _fps; }
	float GetDeltaTime() { return _deltaTime; }

private:
	unsigned __int64 _frequency = 0;
	unsigned __int64 _prevCount = 0;
	float _deltaTime = 0.f;

	double _timeScale;
	const static size_t _maxSampleCount{ 50 };
	float _frameTimeSample[_maxSampleCount];
	float _sampleCount{};
	size_t _currentFps{};
private:
	unsigned __int32 _frameCount = 0;
	float _frameTime = 0.f;
	unsigned __int32 _fps = 0;
};
