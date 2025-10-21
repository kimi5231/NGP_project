#include "pch.h"
#include "GameFramework.h"
#include "GameScene.h"
#include "WindowManager.h"
GameFramework::GameFramework()
{
	_scene = new GameScene();
}

GameFramework::GameFramework(CWindowGameMediator* mediator)
	: _mediator{ mediator }, _scene{ new GameScene(mediator->GetInstance())}
{
}

GameFramework::~GameFramework()
{
	delete _scene;
}

void GameFramework::Init()
{
}

void GameFramework::Update()
{
	_scene->Update();
}

void GameFramework::Render()
{
	_scene->Render();
}

void GameFramework::Render(HDC hDC)
{
	_scene->Render(hDC);
}

void GameFramework::ChangeScene()
{

}