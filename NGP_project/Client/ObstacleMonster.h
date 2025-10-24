#pragma once

class Monster;

class ObstacleMonster : public Monster
{
public:
	ObstacleMonster(HINSTANCE hInst);

	virtual void Init(HINSTANCE hInst) override;
};

