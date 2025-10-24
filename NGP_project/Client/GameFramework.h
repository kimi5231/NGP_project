#pragma once

class Scene;
class CWindowGameMediator;
class InputManager;

class GameFramework
{
public:
	GameFramework();
	GameFramework(CWindowGameMediator* mediator);
	virtual ~GameFramework();

	void Init();
	void Update();
	void Render();
	void Render(HDC hDC);

	void ChangeScene();

private:
	Scene* _scene{};
	CWindowGameMediator* _mediator;
	InputManager* _inputManager;
};