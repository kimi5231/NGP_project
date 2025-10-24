#include "pch.h"
#include "GameFramework.h"
#include "GameScene.h"
#include "WindowManager.h"
#include "InputManager.h"

GameFramework::GameFramework()
{
	_scene = new GameScene();
}

GameFramework::GameFramework(CWindowGameMediator* mediator)
	: _mediator{ mediator }, _scene{ new GameScene(mediator->GetInstance())}, _inputManager{GET_SINGLE(InputManager)}
{
	Init();
}

GameFramework::~GameFramework()
{
	delete _scene;
}

void GameFramework::Init()
{
	_inputManager->Init(_mediator->GetHWND());
}

void GameFramework::Update()
{
	_inputManager->Update();
	_scene->ProcessInput(GET_SINGLE(InputManager));
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