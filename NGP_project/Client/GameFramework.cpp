#include "pch.h"
#include "GameFramework.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "Global.h"
#include "GameNetwork.h"

GameFramework::GameFramework()
{
	Init();
}

GameFramework::~GameFramework()
{
	delete _scene;
}

void GameFramework::Init()
{
	// Get Resource Path
	g_resourcePath = std::filesystem::current_path().parent_path() / "Resource\\Bitmap";

	GET_SINGLE(InputManager)->Init(hWnd);
	GET_SINGLE(TimeManager)->Init();

	_scene = new GameScene;
	_gameNetwork = new GameNetwork;
}

void GameFramework::Update()
{
	GET_SINGLE(InputManager)->Update();
	GET_SINGLE(TimeManager)->Update();

	_scene->Update();

	InvalidateRect(hWnd, NULL, false);
}

void GameFramework::Render(HDC hdc)
{
	_scene->Render(hdc);
}

void GameFramework::ChangeScene()
{

}