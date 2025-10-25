#pragma once
#include "GameObject.h"

class Player : public GameObject 
{
public:
	Player(HINSTANCE hInst);

	virtual void Init(HINSTANCE hInst) override;
	virtual void Update();

public:
	// 상점 아이템과 충돌 시 코인이 충분하면 호출
	void BuyItem(ShopItemType itemType);

	// 나중에 Command 패턴 적용
	void Left();
	void Right();
	void Up();
	void Down();

private:
	int _coin{};
	int _speed{};
	int _attack{};
	int _bulletSpeed{};

	ShopItemType _currentItem{};
};