#include "pch.h"
#include "GameScene.h"
#include "Player.h"

GameScene::GameScene(HINSTANCE hInst)
{
	_players.push_back(std::make_shared<Player>(hInst));
}

void GameScene::Init()
{
}

void GameScene::Update()
{
}

void GameScene::Render()
{
	for (PlayerRef player : _players)
		player->Render();
}

void GameScene::Render(HDC hDC)
{
	for (PlayerRef player : _players)
		player->Render(hDC);
}
