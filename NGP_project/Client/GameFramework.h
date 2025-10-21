#pragma once

class Scene;

class GameFramework
{
public:
	GameFramework();
	virtual ~GameFramework();

	void Init();
	void Update();
	void Render();

	void ChangeScene();

private:
	Scene* _scene{};

};