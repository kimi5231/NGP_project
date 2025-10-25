#pragma once

class Scene;
class InputManager;

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
	Scene* _scene{};
};