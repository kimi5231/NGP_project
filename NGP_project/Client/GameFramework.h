#pragma once

class InputManager;
class GameNetwork;
class GameScene;

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
	GameScene* GetGameScene() { return _scene; }
public:
	void ChangeScene();

private:
	GameScene* _scene{};
	GameNetwork* _gameNetwork{};
	HANDLE _hNetworkThread{};
};