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

	_scene = new TitleScene;
	dynamic_cast<TitleScene*>(_scene)->SetGameFramework(this);
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
	if(_scene)
		_scene->Render(hdc);
}

DWORD __stdcall GameFramework::ProcessNetwork(LPVOID arg)
{
	GameFramework* framework = (GameFramework*)arg;

	while (true)
	{
		if (framework && framework->_gameNetwork)
		{
			framework->_gameNetwork->Update();
		}
	}

	return 0;
}

void GameFramework::ChangeTitleToGameScene(char* ip)
{
	// 삭제
	delete _scene;
	_scene = nullptr;

	// GameScene 생성
	_scene = new GameScene();
	 _gameNetwork = new GameNetwork(ip);
	 dynamic_cast<GameScene*>(_scene)->SetGameNetwork(_gameNetwork);
	_gameNetwork->SetGameScene(dynamic_cast<GameScene*>(_scene));

	// 네트워크 스레드 생성
	_hNetworkThread = CreateThread(NULL, 0, GameFramework::ProcessNetwork, this, 0, NULL);
}