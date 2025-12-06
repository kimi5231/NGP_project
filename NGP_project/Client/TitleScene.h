#pragma once
#include "Scene.h"

class GameObject;
class GameFramework;

class TitleScene : public Scene
{
public:
	TitleScene();
	virtual ~TitleScene();
	virtual void ProcessInput() {};

public:
	virtual void Update();
	virtual void Render(HDC hdc);

public:
	void SetGameFramework(GameFramework* framework) { _framework = framework; }

private:
	HBITMAP _backgroundBitmap{};
	HWND _hEditIP;       // IP 주소를 위한 Edit 컨트롤 핸들
	RECT _menuStartRt;

	GameFramework* _framework;
};