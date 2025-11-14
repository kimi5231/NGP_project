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

public:
	void ChangeScene();

private:
	GameScene* _scene{};
	GameNetwork* _gameNetwork{};
};