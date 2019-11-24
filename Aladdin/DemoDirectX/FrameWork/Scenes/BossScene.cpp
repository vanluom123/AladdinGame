#include "../Scenes/BossScene.h"
#include "../GameObjects/Player/PlayerFallingState.h"

Scene_Final::Scene_Final()
{}

Scene_Final::~Scene_Final()
{
	GameMap::Release();
	delete mPlayer;
	delete mCamera;
	delete mDebugDraw;
	delete playerInfo;
}

Scene_Final::Scene_Final(Player* player)
{
	SceneTag = Scene::SceneName::SceneFinal;
	mDebugDraw = new GameDebugDraw();

	if (player != NULL)
		mPlayer = player;
	else
		mPlayer = new Player();

	mPlayer->SetHealth(11);
	mPlayer->SetPosition(50, 328);
	mPlayer->SetStartPoint(D3DXVECTOR2(50, 128));
	mPlayer->SetRevivePoint(D3DXVECTOR2(-1, -1));

	GameMap::GetInstance()->Initialize("Resources/FinalScene.tmx");
	GameMap::GetInstance()->LoadMapItems("Resources/SceneFinal/Items.txt");
	GameMap::GetInstance()->mPlayer = mPlayer;
	GameMap::GetInstance()->LoadMapObjects("Resources/SceneFinal/Objects_Enemies.txt");


	mCamera = new Camera(GameGlobal::GetWidth(), GameGlobal::GetHeight());
	mCamera->SetPosition(mCamera->GetWidth() / 2, mCamera->GetHeight() / 2);
	GameMap::GetInstance()->SetCamera(mCamera);

	mPlayer->SetCamera(mCamera);

	CheckCameraAndWorldMap();;
	mSpriteHandler = GameGlobal::GetCurrentSpriteHandler();

	playerInfo = new PlayerInfo(mPlayer);
	playerInfo->SetCamera(mCamera);
	PlayMusic();
}


void Scene_Final::Update(float dt)
{
	if (GameMap::GetInstance()->GetEnemies()->size() == 0)
	{
		class EndScene* t = new class EndScene(1, mPlayer);
		SceneManager::GetInstance()->GetCurrentScene()->StopMusic();
		SceneManager::GetInstance()->ReplaceScene(t);
		return;
	}

	if (timeResetApples <= 0)
	{
		GameMap::GetInstance()->ReloadItems();	mCollidable.clear();
		timeResetApples = 60;
	}

	else timeResetApples -= dt;

	if (timeCreateFire <= 0)
	{
		Entity* fire = new Fire(D3DXVECTOR2(530, 301));
		GameMap::GetInstance()->InsertWeapon(fire);
		fire = new Fire(D3DXVECTOR2(563, 301));
		GameMap::GetInstance()->InsertWeapon(fire);
		timeCreateFire = 1.0;
	}
	else
		timeCreateFire -= dt;

	if (mPlayer->getState() != PlayerState::Die && mPlayer->getState() != PlayerState::Revive)
		this->checkCollision(dt);
	else
		if (mPlayer->GetNumLives() < 0)
		{
			Scene* t = new class EndScene(3, mPlayer);
			Sound::getInstance()->stop("Scene1");
			SceneManager::GetInstance()->ReplaceScene(t);
			return;
		}

	GameMap::GetInstance()->Update(dt);
	mPlayer->Update(dt);
	playerInfo->Update(dt);
	mPlayer->HandleKeyboard(keys);
	CheckCameraAndWorldMap();
}

void Scene_Final::Draw()
{
	if (mPlayer->getState() != PlayerState::Die && mPlayer->getState() != PlayerState::Revive)
	{
		GameMap::GetInstance()->Draw();
		playerInfo->Draw();
	}
	else
	{
		GameGlobal::GetCurrentDevice()->Clear(0, NULL, D3DCLEAR_TARGET, 0x000000, 0.0f, 0);
		mPlayer->SetVx(0);
		mPlayer->SetVy(0);
		mPlayer->Draw();
	}

	DrawCollidable();
	mCollidable.clear();
}

void Scene_Final::OnKeyDown(int keyCode)
{
	keys[keyCode] = true;
	mPlayer->OnKeyPressed(keyCode);
}

void Scene_Final::OnKeyUp(int keyCode)
{
	keys[keyCode] = false;
	mPlayer->OnKeyUp(keyCode);
}

void Scene_Final::OnMouseDown(float x, float y)
{
}

void Scene_Final::CheckCameraAndWorldMap()
{
	mCamera->SetPosition(mPlayer->GetPosition().x, (mPlayer->GetPosition().y - mPlayer->GetHeight() / 2));

	if (mCamera->GetBound().left < 0)
	{
		//vi position cua camera ma chinh giua camera
		//luc nay o vi tri goc ben trai cua the gioi thuc
		mCamera->SetPosition(mCamera->GetWidth() / 2, mCamera->GetPosition().y);
	}

	if (mCamera->GetBound().right > GameMap::GetInstance()->GetWidth())
	{
		//luc nay cham goc ben phai cua the gioi thuc
		mCamera->SetPosition(GameMap::GetInstance()->GetWidth() - mCamera->GetWidth() / 2,
			mCamera->GetPosition().y);
	}

	if (mCamera->GetBound().top < 0)
	{
		//luc nay cham goc tren the gioi thuc
		mCamera->SetPosition(mCamera->GetPosition().x, mCamera->GetHeight() / 2);
	}

	if (mCamera->GetBound().bottom > GameMap::GetInstance()->GetHeight())
	{
		//luc nay cham day cua the gioi thuc
		mCamera->SetPosition(mCamera->GetPosition().x,
			GameMap::GetInstance()->GetHeight() - mCamera->GetHeight() / 2);
	}
}

void Scene_Final::checkCollision(float dt)
{

	std::unordered_set<Entity*> listCollision;

#pragma region PLAYER_COLLISION_DETECT

#pragma region PLAYER_WITH_STATIC_OBJECTS
	int widthBottom = 0;

	mCollidable.insert(listCollision.begin(), listCollision.end());

	for (auto collision : listCollision)
	{
		auto r = GameCollision::RectBox(mPlayer->GetBound(), collision->GetBound());

		if (r.IsCollided)
		{

			auto sidePlayer = GameCollision::GetSideCollision(mPlayer, r);
			auto sideImpactor = GameCollision::GetSideCollision(collision, r);

			mPlayer->OnCollision(collision, r, sidePlayer);
			collision->OnCollision(mPlayer, r, sideImpactor);

			if (collision->Tag != Entity::HorizontalRope)
			{
				if (sidePlayer == Entity::Bottom 
					|| sidePlayer == Entity::BottomLeft
					|| sidePlayer == Entity::BottomRight)
				{
					int bot = r.RegionCollision.right - r.RegionCollision.left;

					if (bot > widthBottom)
						widthBottom = bot;
				}
			}
		}
	}

#pragma endregion PLAYER_WITH_STATIC_OBJECTS

#pragma region PLAYER_WITH_ENEMIES_WEAPONS

	for (auto it : (*GameMap::GetInstance()->GetWeapons()))
	{
		if (it->IsDestroy() == true)
			continue;

		auto r = GameCollision::RectBox(mPlayer->GetBound(), it->GetBound());
		if (r.IsCollided)
		{
			mPlayer->OnCollision(it, r, r.sideCollision);
			it->OnCollision(mPlayer, r, r.sideCollision);
		}
	}

#pragma endregion PLAYER_WITH_ENEMIES_WEAPONS

#pragma region PLAYER_WITH_ENEMIES
	// Xét va chạm với Enemies

	for (auto it : (*GameMap::GetInstance()->GetEnemies()))
	{
		if (it->IsDestroy() == true) continue;
		auto r = GameCollision::RectBox(mPlayer->GetBound(), it->GetBound());

		if (r.IsCollided)
		{

			auto sideImpactor = GameCollision::GetSideCollision(it, r);
			it->OnCollision(mPlayer, r, sideImpactor);


			if (it->Tag == Entity::FloatingGround 
				|| it->Tag == Entity::Camel 
				|| it->Tag == Entity::SpringBoard)
			{
				auto sidePlayer = GameCollision::GetSideCollision(mPlayer, r);
				mPlayer->OnCollision(it, r, sidePlayer);

				if (sidePlayer == Entity::Bottom 
					|| sidePlayer == Entity::BottomLeft
					|| sidePlayer == Entity::BottomRight)
				{
					int bot = r.RegionCollision.right - r.RegionCollision.left;

					if (bot > widthBottom)
						widthBottom = bot;
				}
			}
		}
	}


	if (widthBottom < Define::PLAYER_BOTTOM_RANGE_FALLING)
	{
		if (mPlayer->getState() != PlayerState::Climb)
			mPlayer->allowJump = false;
		mPlayer->OnNoCollisionWithBottom(dt);
	}
	else
		mPlayer->allowJump = true;


#pragma endregion  PLAYER_WITH_ENEMIES

#pragma endregion PLAYER_COLLISION_DETECT



#pragma region APPLE_COLLISION_DETECT
	for (auto it : (*GameMap::GetInstance()->GetApples()))
	{
		if (it->IsDestroy() == true) continue;

#pragma region APPLE_WITH_STATIC_OBJECT

		for (auto collision : listCollision)
		{
			auto r = GameCollision::RectBox(it->GetBound(), collision->GetBound());

			if (r.IsCollided)
			{
				it->OnCollision(collision, r, r.sideCollision);
				collision->OnCollision(it, r, r.sideCollision);
			}
		}
#pragma endregion APPLE_WITH_STATIC_OBJECT	

#pragma region APPLE_WITH_ENEMIES
		for (auto enemy : (*GameMap::GetInstance()->GetEnemies()))
		{
			if (enemy->IsDestroy() == true) continue;

			auto r = GameCollision::RectBox(it->GetBound(), enemy->GetBound());

			if (r.IsCollided)
			{
				enemy->OnCollision(it, r, r.sideCollision);
				it->OnCollision(enemy, r, r.sideCollision);
			}
		}
#pragma endregion APPLE_WITH_ENEMIES

#pragma region APPLE_WITH_ENEMIES_WEAPONS
		for (auto weaponEnemy : (*GameMap::GetInstance()->GetWeapons()))
		{
			if (weaponEnemy->IsDestroy() == true) continue;

			auto r = GameCollision::RectBox(it->GetBound(), weaponEnemy->GetBound());

			if (r.IsCollided)
			{
				weaponEnemy->OnCollision(it, r, r.sideCollision);
				it->OnCollision(weaponEnemy, r, r.sideCollision);
			}
		}
#pragma endregion APPLE_WITH_ENEMIES_WEAPONS

	}
#pragma endregion APPLE_COLLISION_DETECT



#pragma region ENEMIESWEAPONS_COLLISION_DETECT

#pragma region ENEMIESWEAPONS_WITH_STATIC_OBJECTS
	for (auto itWeapon : (*GameMap::GetInstance()->GetEnemies()))
	{
		if (itWeapon->IsDestroy())
			continue;

		for (auto collision : listCollision)
		{
			auto r = GameCollision::RectBox(itWeapon->GetBound(), collision->GetBound());

			if (r.IsCollided)
			{
				auto sideEnemyWeapon = GameCollision::GetSideCollision(mPlayer, r);
				auto sideImpactor = GameCollision::GetSideCollision(collision, r);
				itWeapon->OnCollision(collision, r, sideEnemyWeapon);
				collision->OnCollision(itWeapon, r, sideImpactor);
			}
		}
	}
#pragma endregion APPLE_WITH_STATIC_OBJECT	

#pragma endregion ENEMIESWEAPONS_COLLISION_DETECT


#pragma region INSERT_COLLIDABLE_GAME_DEBUG

	mCollidable.insert(GameMap::GetInstance()->GetEnemies()->begin(), GameMap::GetInstance()->GetEnemies()->end());
	mCollidable.insert(GameMap::GetInstance()->GetApples()->begin(), GameMap::GetInstance()->GetApples()->end());
	mCollidable.insert(GameMap::GetInstance()->GetWeapons()->begin(), GameMap::GetInstance()->GetWeapons()->end());
	mCollidable.insert(mPlayer);

#pragma endregion INSERT_COLLIDABLE_GAME_DEBUG

}


void Scene_Final::DrawCollidable()
{
	for (auto child : mCollidable)
	{
		mDebugDraw->DrawRect(child->GetBound(), mCamera);
	}
}
void Scene_Final::PlayMusic()
{
	Sound::getInstance()->play("BossTune", true, 1);
}

void Scene_Final::StopMusic()
{
	Sound::getInstance()->stop("BossTune");
}

Scene::SceneName Scene_Final::GetSceneName()
{
	return Scene::SceneName::SceneFinal;
}