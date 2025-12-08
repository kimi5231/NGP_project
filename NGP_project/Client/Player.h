#pragma once
#include "GameObject.h"

class Item;
class TextureUI;

class Player : public GameObject 
{
public:
	Player();
public:
	virtual void Update();
	void UseItem();
	void SetItem(ItemType item);
	void Move(Vertex vecDir, Dir dir);
	void Render(HDC hdc, HDC srcDC) override;

	void SetDirAndFrame(Dir dir);

	void ClampPlayerArea();

	Vertex GetPrevSendPos() const { return _prevSendPos; }
	void   SetPrevSendPos(const Vertex& pos) { _prevSendPos = pos; }

	Dir GetPrevDir() const { return _prevDir; }
	void SetPrevDir(Dir dir) { _prevDir = dir; }

	ItemRef& GetItem()  {return _item.first;}

	std::shared_ptr<TextureUI> _itemUI;
private:
	std::pair<ItemRef, ItemRef> _item;	// first: 가지고 있는 아이템, second: 사용 중인 아이템
	float _itemTimer{};

	Vertex _prevSendPos{};
	Dir _prevDir{};
};

