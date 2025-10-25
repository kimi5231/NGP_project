#pragma once

class Monster;

class ObstacleMonster : public Monster
{
public:
	ObstacleMonster();

	virtual void Init() override;
};

