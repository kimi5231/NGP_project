#include "pch.h"
#include "GameFramework.h"
#include "GameScene.h"
#include "WindowManager.h"
#include "InputManager.h"

GameFramework::GameFramework()
{

}

GameFramework::GameFramework(CWindowGameMediator* mediator)
	: _mediator{ mediator }
{
	Init();
}

GameFramework::~GameFramework()
{
	delete _scene;
}

void GameFramework::Init()
{
	GET_SINGLE(InputManager)->Init(_mediator->GetHWND());

	_scene = new GameScene(_mediator->GetInstance());
}

void GameFramework::Update()
{
	GET_SINGLE(InputManager)->Update();

	_scene->Update();
}

void GameFramework::Render(HDC hdc)
{
	_scene->Render(hdc);
}

void GameFramework::ChangeScene()
{

}