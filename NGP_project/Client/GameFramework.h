#pragma once

class Scene;
class CWindowGameMediator;

class GameFramework
{
public:
	GameFramework();
	GameFramework(CWindowGameMediator* mediator);
	virtual ~GameFramework();

	void Init();
	void Update();
	void Render();

	void ChangeScene();

private:
	Scene* _scene{};
	CWindowGameMediator* _mediator;
};