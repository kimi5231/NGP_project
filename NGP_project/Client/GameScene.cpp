#include "pch.h"
#include "GameObject.h"
#include "GameScene.h"

GameScene::GameScene(HINSTANCE hInst)
{
	//_players.push_back(std::make_shared<Player>(hInst));
	_objects.push_back(new GameObject(hInst));
}

GameScene::~GameScene()
{
	for (GameObject* object : _objects) {
		delete object;
	}
}

void GameScene::Init()
{
}

void GameScene::Update()
{
}

void GameScene::Render()
{

}

void GameScene::Render(HDC hDC)
{
	/*for (PlayerRef player : _players)
		player->Render(hDC);*/
	for (GameObject* object : _objects) {
		object->Render(hDC);
	}
}
