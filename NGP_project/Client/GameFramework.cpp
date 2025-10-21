#include "pch.h"
#include "GameFramework.h"
#include "GameScene.h"

GameFramework::GameFramework()
{
	_scene = new GameScene();
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

void GameFramework::ChangeScene()
{

}