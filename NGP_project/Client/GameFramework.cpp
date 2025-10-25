#include "pch.h"
#include "GameFramework.h"
#include "GameScene.h"
#include "InputManager.h"
#include "Global.h"

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
	// 상대 경로 알아내기
	g_resourcePath = std::filesystem::current_path().parent_path() / "Resource\\Bitmap";

	GET_SINGLE(InputManager)->Init(hWnd);

	_scene = new GameScene();
}

void GameFramework::Update()
{
	GET_SINGLE(InputManager)->Update();

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