#pragma once

class Scene;
class CWindowGameMediator;
class InputManager;

class GameFramework
{
public:
	GameFramework(CWindowGameMediator* mediator);
	virtual ~GameFramework();

public:
	void Init();
	void Update();
	void Render(HDC hdc);

public:
	void ChangeScene();

private:
	Scene* _scene{};
	CWindowGameMediator* _mediator;
};