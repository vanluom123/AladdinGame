#include "BatEnemy.h"

BatEnemy::BatEnemy()
{
	this->Tag = Entity::BatEnemy;
	this->mBatEnemyData = new BatEnemyData();
	this->mBatEnemyData->batEnemy = this;
	mCurrentAnimation = new Animation();
	this->mBatEnemyData->batEnemy->SetState(new BatEnemyFlyState(this->mBatEnemyData));
}

BatEnemy::BatEnemy(D3DXVECTOR2 position, Player* mplayer)
{
	this->isDraw = true;
	this->isDeleted = false;
	this->isDestroy = false;

	this->Tag = Entity::BatEnemy;
	this->HP = 1;
	this->mBatEnemyData = new BatEnemyData();
	this->mBatEnemyData->batEnemy = this;
	mCurrentAnimation = new Animation(eID::BATENEMY, "BatEnemy", 1, 0.5f, NULL);
	this->mBatEnemyData->batEnemy->SetState(new BatEnemyFlyState(this->mBatEnemyData));
	this->SetPosition(position);
	this->mPlayer = mplayer;
	this->GetCurrentAnimation()->GetSprite()->SetAnchorPoint(0, 0);
}

BatEnemy::~BatEnemy()
{
}

void BatEnemy::SetGameMap(GameMap * gamemap)
{
	this->mGamMap = gamemap;
}

GameMap * BatEnemy::getGameMap()
{
	return this->mGamMap;
}

void BatEnemy::Update(float dt)
{
	if (this->mCurrentAnimation->mCurrentIndex == this->mCurrentAnimation->mTotalFrame && isDestroy == true)
		isDeleted = true;

	if (IsNearToPlayer() == true)
	{
		mCurrentAnimation->Update(dt);
		if (this->mBatEnemyData->state)
		{
			this->mBatEnemyData->state->Update(dt);
		}

		Entity::Update(dt);
	}
	else
	{
		this->mBatEnemyData->batEnemy->SetState(new BatEnemyFlyState(mBatEnemyData));
		this->GetCurrentAnimation()->GetSprite()->SetAnchorPoint(0, 0);
	}
}

void BatEnemy::SetState(BatEnemyState *newState)
{
	delete this->mBatEnemyData->state;
	this->mBatEnemyData->state = newState;
	this -> changeAnimation(newState->GetState());
	mCurrentState = newState->GetState();
}

void BatEnemy::Draw(D3DXVECTOR3 position, RECT sourceRect, D3DXVECTOR2 scale, D3DXVECTOR2 transform, float angle, D3DXVECTOR2 rotationCenter, D3DXCOLOR colorKey)
{
	if (IsNearToPlayer() == true)
	{
		mCurrentAnimation->SetPosition(this->GetPosition());
		mCurrentAnimation->Draw(position, sourceRect, scale, transform, angle, rotationCenter, colorKey);
	}
}

void BatEnemy::Draw(D3DXVECTOR2 transform)
{
	if (IsNearToPlayer() == true)
	{
		mCurrentAnimation->SetPosition(this->GetPosition());
		mCurrentAnimation->Draw(this->GetPosition(), RECT(), D3DXVECTOR2(), transform);
	}
}

void BatEnemy::OnCollision(Entity * impactor, Entity::CollisionReturn data, Entity::SideCollisions side)
{
	switch (impactor->Tag)
	{
	case Entity::Aladdin:
		if (impactor->IsMakeDamage() == true && GameCollision::AABBCheck(impactor->GetBoundWeapon(), this->GetBound()) == true)
			this->TakeDamage(1);
		break;
	}
	if (this->HP <= 0)
	{
		this->isDestroy = true;
		delete mCurrentAnimation;
		mCurrentAnimation = new Animation(eID::EXPLOSION, "Explosion3rd", 5, 0.15f, NULL);
		Sound::getInstance()->play("BalloonPop", false, 1);
	}
}

RECT BatEnemy::GetBound()
{
	POINT tmp = this->mBatEnemyData->batEnemy->GetCurrentAnimation()->GetAnchorPoint();
	RECT rect;
	if (tmp.x == -1 && tmp.y == -1)
	{
		rect.left = this->posX - mCurrentAnimation->GetSprite()->GetWidth() / 2;
		rect.right = rect.left + mCurrentAnimation->GetSprite()->GetWidth();
		rect.top = this->posY - mCurrentAnimation->GetSprite()->GetHeight();
		rect.bottom = rect.top + mCurrentAnimation->GetSprite()->GetHeight();
	}
	else
	{
		{
			rect.left = this->posX - tmp.x;
			rect.right = rect.left + mCurrentAnimation->GetSprite()->GetWidth();
		}
		rect.top = this->posY - tmp.y;
		rect.bottom = rect.top + mCurrentAnimation->GetSprite()->GetHeight();
	}
	return rect;
}

Animation * BatEnemy::GetCurrentAnimation()
{
	return mCurrentAnimation;
}

Animation * BatEnemy::GetAnimation(BatEnemyState::StateName state)
{
	switch (state)
	{
	case BatEnemyState::fly:
		return mAnimationFly;
	
	default:
		break;
	}
	return nullptr;
}

bool BatEnemy::IsNearToPlayer()
{
	int deltaX = this->mPlayer->GetPosition().x - this->mBatEnemyData->batEnemy->GetPosition().x;
	int deltaY = this->mPlayer->GetPosition().y - this->mBatEnemyData->batEnemy->GetPosition().y;

	if (abs(deltaX) <= 70 && deltaY > 0 && deltaY < 200)
	{
		return true;
	}
	return false;
}

void BatEnemy::OnSetPosition(D3DXVECTOR3 pos)
{
	mCurrentAnimation->SetPosition(pos);
}

void BatEnemy::changeAnimation(BatEnemyState::StateName state)
{
	switch (state)
	{
	case BatEnemyState::fly:
		delete mCurrentAnimation;
		mCurrentAnimation = new Animation(eID::BATENEMY, "BatEnemy", 4, 0.2f, NULL);
		break;
	default:
		break;
	}
}
