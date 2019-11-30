#include "Bat.h"

Bat::Bat()
{
	Tag = Entity::BatEnemy;
	m_data = BATDATA();
	m_data.batEnemy = this;
	m_CurrentAnimation = new Animation();
	m_data.batEnemy->ChangeState(new BatEnemyFlyState(m_data));
}

Bat::Bat(D3DXVECTOR2 position, Player* pPlayer)
{
	isDraw = true;
	isDeleted = false;
	isDestroy = false;
	Tag = Entity::BatEnemy;
	HP = 1;
	m_data = BATDATA();
	m_data.batEnemy = this;
	m_CurrentAnimation = new Animation(eID::BATENEMY, "BatEnemy", 1, 0.5f, NULL);
	m_data.batEnemy->ChangeState(new BatEnemyFlyState(m_data));
	SetPosition(position);
	m_pPlayer = pPlayer;
	m_CurrentAnimation->GetSprite()->SetAnchorPoint(0, 0);
}

Bat::~Bat()
{
}

void Bat::SetGameMap(GameMap* gamemap)
{
	m_pGamMap = gamemap;
}

GameMap* Bat::getGameMap()
{
	return m_pGamMap;
}

void Bat::Update(float dt)
{
	if (m_CurrentAnimation->mCurrentIndex == m_CurrentAnimation->mTotalFrame && isDestroy == true)
		isDeleted = true;

	if (IsNearToPlayer() == true)
	{
		m_CurrentAnimation->Update(dt);

		if (m_data.state)
			m_data.state->Update(dt);

		Entity::Update(dt);
	}
	else
	{
		m_data.batEnemy->ChangeState(new BatEnemyFlyState(m_data));
		GetCurrentAnimation()->GetSprite()->SetAnchorPoint(0, 0);
	}
}

void Bat::ChangeState(BatState* newState)
{
	delete m_data.state;
	m_data.state = newState;
	changeAnimation(newState->GetState());
	m_currentState = newState->GetState();
}

void Bat::Draw(D3DXVECTOR3 position, RECT sourceRect, D3DXVECTOR2 scale, D3DXVECTOR2 transform, float angle, D3DXVECTOR2 rotationCenter, D3DXCOLOR colorKey)
{
	if (IsNearToPlayer() == true)
	{
		m_CurrentAnimation->SetPosition(GetPosition());
		m_CurrentAnimation->Draw(position, sourceRect, scale, transform, angle, rotationCenter, colorKey);
	}
}

void Bat::Draw(D3DXVECTOR2 transform)
{
	if (IsNearToPlayer() == true)
	{
		m_CurrentAnimation->SetPosition(GetPosition());
		m_CurrentAnimation->Draw(GetPosition(), RECT(), D3DXVECTOR2(), transform);
	}
}

void Bat::OnCollision(Entity* impactor, Entity::CollisionReturn data, Entity::SideCollisions side)
{
	if (isDeleted || HP <= 0)
		return;

	switch (impactor->Tag)
	{
	case Entity::Aladdin:
	{
		switch (((Player*)impactor)->getState())
		{
			if (((Player*)impactor)->IsMakeDamage() == true && GameCollision::AABBCheck(((Player*)impactor)->GetBoundWeapon(), GetBound()) == true)
				TakeDamage(1);
		}
	}
	break;
	default: break;
	}

	if (HP <= 0)
	{
		isDestroy = true;
		delete m_CurrentAnimation;
		m_CurrentAnimation = new Animation(eID::EXPLOSION, "Explosion3rd", 5, 0.15f, NULL);
		Sound::getInstance()->play("BalloonPop", false, 1);
	}
}

RECT Bat::GetBound()
{
	POINT tmp = m_data.batEnemy->GetCurrentAnimation()->GetAnchorPoint();
	RECT rect;
	if (tmp.x == -1 && tmp.y == -1)
	{
		rect.left = posX - m_CurrentAnimation->GetSprite()->GetWidth() / 2;
		rect.right = rect.left + m_CurrentAnimation->GetSprite()->GetWidth();
		rect.top = posY - m_CurrentAnimation->GetSprite()->GetHeight();
		rect.bottom = rect.top + m_CurrentAnimation->GetSprite()->GetHeight();
	}
	else
	{
		rect.left = posX - tmp.x;
		rect.right = rect.left + m_CurrentAnimation->GetSprite()->GetWidth();
		rect.top = posY - tmp.y;
		rect.bottom = rect.top + m_CurrentAnimation->GetSprite()->GetHeight();
	}
	return rect;
}

Animation* Bat::GetCurrentAnimation()
{
	return m_CurrentAnimation;
}

Animation* Bat::GetAnimation(BatState::StateName state)
{
	switch (state) {
	case BatState::fly:
		return m_animationFly;

	default: return nullptr;
	}
}

bool Bat::IsNearToPlayer()
{
	int deltaX = m_pPlayer->GetPosition().x - m_data.batEnemy->GetPosition().x;
	int deltaY = m_pPlayer->GetPosition().y - m_data.batEnemy->GetPosition().y;

	return ((abs(deltaX) <= 70) && (deltaY > 0) && (deltaY < 200));
}

void Bat::OnSetPosition(D3DXVECTOR3 pos)
{
	m_CurrentAnimation->SetPosition(pos);
}

void Bat::changeAnimation(BatState::StateName state)
{
	switch (state)
	{
	case BatState::fly:
		delete m_CurrentAnimation;
		m_CurrentAnimation = new Animation(eID::BATENEMY, "BatEnemy", 4, 0.2f, NULL);
		break;
	default:
		break;
	}
}
