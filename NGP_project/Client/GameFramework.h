#pragma once

class InputManager;
class GameNetwork;
class Scene;

class GameFramework
{
public:
	GameFramework();
	virtual ~GameFramework();

public:
	void Init();
	void Update();
	void Render(HDC hdc);

	static DWORD WINAPI ProcessNetwork(LPVOID arg);

public:
	Scene* GetGameScene() { return _scene; }

public:
	void ChangeTitleToGameScene(char* ip);

private:
	Scene* _scene{};
	GameNetwork* _gameNetwork{};
	HANDLE _hNetworkThread{};
};