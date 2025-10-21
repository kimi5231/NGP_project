#pragma once
class Scene
{
public:
	Scene() {};
	virtual ~Scene() {};

public:
	virtual void Init() {};
	virtual void Update() {};
	virtual void Render() {};
	virtual void Render(HDC) {};
};