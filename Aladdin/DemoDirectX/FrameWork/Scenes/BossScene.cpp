#include "../Scenes/BossScene.h"
#include "../GameObjects/Player/PlayerFallingState.h"
Scene_Final::Scene_Final()
{
}
Scene_Final::Scene_Final(Player* player)
{
	SceneTag = Scene::SceneName::SceneFinal;
	mDebugDraw = new GameDebugDraw();


	if (player != NULL)mPlayer = player;
	else mPlayer = new Player();
	mPlayer->SetHealth(11);
	mPlayer->SetPosition(50, 328);
	mPlayer->SetStartPoint(D3DXVECTOR2(50, 128));
	mPlayer->SetRevivePoint(D3DXVECTOR2(-1, -1));


	mMap = new GameMap("Resources/FinalScene.tmx");
	mMap->LoadMapItems("Resources/SceneFinal/Items.txt");
	mMap->mPlayer = mPlayer;
	mMap->LoadMapObjects("Resources/SceneFinal/Objects_Enemies.txt");


	mCamera = new Camera(GameGlobal::GetWidth(), GameGlobal::GetHeight());
	mCamera->SetPosition(mCamera->GetWidth() / 2, mCamera->GetHeight() / 2);
	mMap->SetCamera(mCamera);


	mPlayer->SetCamera(mCamera);
	mPlayer->SetGameMap(mMap);

	CheckCameraAndWorldMap();;
	mSpriteHandler = GameGlobal::GetCurrentSpriteHandler();

	playerInfo = new PlayerInfo(mPlayer);
	playerInfo->SetCamera(mCamera);
	PlayMusic();

	mListApplesAladdin = mMap->GetListApples();
	mListEnemies = mMap->GetListEnemies();
	mListEnemyWeapons = mMap->GetListWeapons();
}


void Scene_Final::Update(float dt)
{
	if (mListEnemies->size() == 0)
	{
		class EndScene* t = new class EndScene(1, mPlayer);
		SceneManager::GetInstance()->GetCurrentScene()->StopMusic();
		SceneManager::GetInstance()->ReplaceScene(t);
		return;
	}

	if (timeResetApples <= 0)
	{
		mMap->ReloadItems();	mCollidable.clear();
		timeResetApples = 60;
	}

	else timeResetApples -= dt;

	if (timeCreateFire <= 0)
	{
		Entity* fire = new Fire(D3DXVECTOR2(530, 301));
		mMap->InsertWeapon(fire);
		fire = new Fire(D3DXVECTOR2(563, 301));
		mMap->InsertWeapon(fire);
		timeCreateFire = 1.0;
	}

	else timeCreateFire -= dt;

	if (mPlayer->getState() != PlayerState::Die && mPlayer->getState() != PlayerState::Revive) //Trạng thái không xét va chạm
		checkCollision(dt);
	else if (mPlayer->GetNumLives() < 0)				//Hết số mạng chuyển về màn hình EndScene:GameOver
	{
		Scene* t = new class EndScene(3, mPlayer);		//EndScene GameOver
		Sound::getInstance()->stop("Scene1");
		SceneManager::GetInstance()->ReplaceScene(t);
		return;
	}
	mMap->Update(dt);
	mPlayer->Update(dt);
	playerInfo->Update(dt);
	mPlayer->HandleKeyboard(keys);
	CheckCameraAndWorldMap();
}

void Scene_Final::Draw()
{
	if (mPlayer->getState() != PlayerState::Die && mPlayer->getState() != PlayerState::Revive)
	{
		mMap->Draw();
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

	if (mCamera->GetBound().right > mMap->GetWidth())
	{
		//luc nay cham goc ben phai cua the gioi thuc
		mCamera->SetPosition(mMap->GetWidth() - mCamera->GetWidth() / 2,
			mCamera->GetPosition().y);
	}

	if (mCamera->GetBound().top < 0)
	{
		//luc nay cham goc tren the gioi thuc
		mCamera->SetPosition(mCamera->GetPosition().x, mCamera->GetHeight() / 2);
	}

	if (mCamera->GetBound().bottom > mMap->GetHeight())
	{
		//luc nay cham day cua the gioi thuc
		mCamera->SetPosition(mCamera->GetPosition().x,
			mMap->GetHeight() - mCamera->GetHeight() / 2);
	}
}

void Scene_Final::checkCollision(float dt)
{
	unordered_set<Entity*> listCollision;

#pragma region PLAYER_COLLISION_DETECT

#pragma region PLAYER_WITH_STATIC_OBJECTS
	int widthBottom = 0;
	mMap->GetGrid()->GetListEntity(listCollision, mCamera);

	for (auto child : listCollision)
		mCollidable.insert(child);

	for (auto objectCollision : listCollision)
	{
		auto r = GameCollision::rectCollide(mPlayer->GetBound(), objectCollision->GetBound());

		if (r.IsCollided)
		{
			//lay phia va cham cua Entity so voi Player
			auto sidePlayer = GameCollision::getSideCollision(mPlayer, r);

			//lay phia va cham cua Player so voi Entity
			auto sideImpactor = GameCollision::getSideCollision(objectCollision, r);

			//goi den ham xu ly collision cua Player va Entity
			mPlayer->OnCollision(objectCollision, r, sidePlayer);
			objectCollision->OnCollision(mPlayer, r, sideImpactor);

			if (objectCollision->Tag != Entity::HorizontalRope)
			{
				//kiem tra neu va cham voi phia duoi cua Player 
				if (sidePlayer == Entity::Bottom
					|| sidePlayer == Entity::BottomLeft
					|| sidePlayer == Entity::BottomRight)
				{
					//kiem tra do dai  tiep xuc phia duoi day
					int bot = r.RegionCollision.right - r.RegionCollision.left;

					if (bot > widthBottom)
						widthBottom = bot;
				}
			}
		}
	}

#pragma endregion PLAYER_WITH_STATIC_OBJECTS

#pragma region PLAYER_WITH_ENEMIES_WEAPONS
	//Xét va chạm với vũ khí của Enemies.
	{
		for (auto enemyWeapons : (*mListEnemyWeapons))
		{
			if (enemyWeapons->IsDestroy() == true)
				continue;

			auto r = GameCollision::rectCollide(mPlayer->GetBound(),
				enemyWeapons->GetBound());

			if (r.IsCollided)
			{
				mPlayer->OnCollision(enemyWeapons, r, r.sideCollision);
				enemyWeapons->OnCollision(mPlayer, r, r.sideCollision);
			}
		}
	}
#pragma endregion PLAYER_WITH_ENEMIES_WEAPONS

#pragma region PLAYER_WITH_ENEMIES

	// Xét va chạm với Enemies
	for (auto enemies : (*mListEnemies))
	{
		if (enemies->IsDestroy() == true) continue;
		auto r = GameCollision::rectCollide(mPlayer->GetBound(), enemies->GetBound());

		if (r.IsCollided)
		{
			//lay phia va cham cua Player so voi Entity
			auto sideImpactor = GameCollision::getSideCollision(enemies, r);

			//Xử lý của Enemies và Player đều làm ở trong này.
			enemies->OnCollision(mPlayer, r, sideImpactor);

			if (enemies->Tag == Entity::FloatingGround || enemies->Tag == Entity::Camel || enemies->Tag == Entity::SpringBoard)
			{
				//lay phia va cham cua Entity so voi Player
				auto sidePlayer = GameCollision::getSideCollision(mPlayer, r);
				mPlayer->OnCollision(enemies, r, sidePlayer);

				if (sidePlayer == Entity::Bottom
					|| sidePlayer == Entity::BottomLeft
					|| sidePlayer == Entity::BottomRight)
				{
					//kiem tra do dai  tiep xuc phia duoi day
					int bot = r.RegionCollision.right - r.RegionCollision.left;

					if (bot > widthBottom)
						widthBottom = bot;
				}
			}
		}
	}


	//Neu  dung ngoai mep thi luc nay cho Aladdin rot xuong duoi dat    
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


	for (auto appleAladdin : (*mListApplesAladdin))
	{
		if (appleAladdin->IsDestroy() == true)
			continue;
#pragma region APPLE_WITH_STATIC_OBJECT

		listCollision.clear();

		for (auto objectCollision : listCollision)
		{
			auto r = GameCollision::rectCollide(appleAladdin->GetBound(), objectCollision->GetBound());

			if (r.IsCollided)
			{
				//goi den ham xu ly collision cua Apple va Entity
				appleAladdin->OnCollision(objectCollision, r, r.sideCollision);
				objectCollision->OnCollision(appleAladdin, r, r.sideCollision);
			}
		}

#pragma endregion APPLE_WITH_STATIC_OBJECT	

#pragma region APPLE_WITH_ENEMIES

		for (auto currentEnemy : (*mListEnemies))
		{
			if (currentEnemy->IsDestroy() == true) continue;

			auto r = GameCollision::rectCollide(appleAladdin->GetBound(), currentEnemy->GetBound());

			if (r.IsCollided)
			{
				//goi den ham xu ly collision cua Apple va Entity
				//Đổi thứ tự xét Apple sau Enemies để Apple gây sát thương.
				currentEnemy->OnCollision(appleAladdin, r, r.sideCollision);
				appleAladdin->OnCollision(currentEnemy, r, r.sideCollision);
			}
		}

#pragma endregion APPLE_WITH_ENEMIES

#pragma region APPLE_WITH_ENEMIES_WEAPONS

		for (auto currentEnemyWeapon : (*mListEnemyWeapons))
		{
			if (currentEnemyWeapon->IsDestroy() == true)
				continue;

			auto r = GameCollision::rectCollide(appleAladdin->GetBound(), currentEnemyWeapon->GetBound());

			if (r.IsCollided)
			{
				//goi den ham xu ly collision cua Apple va Entity
				//Đổi thứ tự xét Apple sau Enemies để Apple gây sát thương.
				currentEnemyWeapon->OnCollision(appleAladdin, r, r.sideCollision);
				appleAladdin->OnCollision(currentEnemyWeapon, r, r.sideCollision);
			}
		}

#pragma endregion APPLE_WITH_ENEMIES_WEAPONS

	}
#pragma endregion APPLE_COLLISION_DETECT

#pragma region ENEMIESWEAPONS_COLLISION_DETECT

#pragma region ENEMIESWEAPONS_WITH_STATIC_OBJECTS

	for (auto currentEnemyWeapon : (*mListEnemyWeapons))
	{
		if (currentEnemyWeapon->IsDestroy())
			continue;

		listCollision.clear();

		for (auto currentCollision : listCollision)
		{
			auto r = GameCollision::rectCollide(currentEnemyWeapon->GetBound(), currentCollision->GetBound());

			if (r.IsCollided)
			{
				//lay phia va cham cua Entity so voi Player
				auto sideEnemyWeapon = GameCollision::getSideCollision(mPlayer, r);

				//lay phia va cham cua Player so voi Entity
				auto sideImpactor = GameCollision::getSideCollision(currentCollision, r);

				//goi den ham xu ly collision cua Apple va Entity
				currentEnemyWeapon->OnCollision(currentCollision, r, sideEnemyWeapon);
				currentCollision->OnCollision(currentEnemyWeapon, r, sideImpactor);
			}
		}
	}
#pragma endregion APPLE_WITH_STATIC_OBJECT	

#pragma endregion ENEMIESWEAPONS_COLLISION_DETECT


#pragma region INSERT_COLLIDABLE_GAME_DEBUG
	for (auto enemy : (*mListEnemies))
		mCollidable.insert(enemy);

	for (auto apple : (*mListApplesAladdin))
		mCollidable.insert(apple);

	for (auto weapon : (*mListEnemyWeapons))
		mCollidable.insert(weapon);

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