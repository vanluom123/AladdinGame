#pragma once

#include "../GameComponents/Animation.h"
#include "Entity.h"
//vien gach
#include "Player\Player.h"
#include "../GameComponents/Text.h"

class PlayerInfo
{
public:

	~PlayerInfo();
	PlayerInfo();
	PlayerInfo(Player* player);

	void Draw();
	void Update(float dt);

	void SetCamera(Camera* camera);

protected:
	bool init(Player* player);
	int playerHP;
	Animation* healthMeter;
	GVector3 healthMeterPosition;

	Animation* Apple;
	GVector3 ApplePosition;

	Animation* Money;
	GVector3 MoneyPosition;

	Animation* UP;
	GVector3 UPPosition;

	Text numApples;
	GVector3 numApplesPosition;

	Text numLives;
	GVector3 numLivesPosition;

	Text numMoney;
	GVector3 numMoneyPosition;

	Text numScore;
	GVector3 numScorePosition;

	Player* player;
	Camera* mCamera;
	GVector3 position;
};
