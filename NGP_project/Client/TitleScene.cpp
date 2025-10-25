#include "pch.h"
#include "TitleScene.h"
#include "Global.h"
#include "GameObject.h"

TitleScene::TitleScene()
{
	// Create Title
	GameObjectRef title = std::make_shared<GameObject>();
	
	HBITMAP bitmap = (HBITMAP)LoadImage(hInst, (g_resourcePath / "Title.bmp").wstring().c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	title->SetObjectType(ObjectType::Background);
	title->SetBitmap(bitmap);
	title->SetSpriteCount({0,0});
	
	_objects.push_back(title);
}

TitleScene::~TitleScene()
{
}

void TitleScene::Render(HDC hdc)
{
	Scene::Render(hdc);
}