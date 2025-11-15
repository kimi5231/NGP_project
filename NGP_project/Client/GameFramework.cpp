#include "pch.h"
#include "GameFramework.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "SoundManager.h"
#include "Global.h"
#include "GameNetwork.h"

GameFramework::GameFramework()
{
}

GameFramework::~GameFramework()
{
	delete _scene;
}

void GameFramework::Init()
{
	// Get Resource Path
	g_resourcePath = std::filesystem::current_path().parent_path() / "Resource\\Bitmap";
	std::filesystem::path soundPath = std::filesystem::current_path().parent_path() / "Resource\\Sound";

	GET_SINGLE(InputManager)->Init(hWnd);
	GET_SINGLE(TimeManager)->Init();
	
	// Sound
	GET_SINGLE(SoundManager)->Init(hWnd, soundPath);
	GET_SINGLE(SoundManager)->LoadSound(L"main_music", L"main_music.wav", SoundType::BGM);

	_gameNetwork = new GameNetwork;

	_scene = new GameScene;
	_gameNetwork->SetGameScene(_scene);
}

void GameFramework::Update()
{
	GET_SINGLE(InputManager)->Update();
	GET_SINGLE(TimeManager)->Tick(60);

	_scene->Update();
	//_gameNetwork->Update();

	InvalidateRect(hWnd, NULL, false);
}

void GameFramework::Render(HDC hdc)
{
	if(_scene)
		_scene->Render(hdc);
}

void GameFramework::ChangeScene()
{

}