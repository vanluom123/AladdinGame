#include "../Scenes/Scene1.h"
#include "../GameObjects/Player/PlayerClimbingState.h"
#include "../GameObjects/MapObject/BrickDynamic.h"
Scene1::Scene1()
{
	LoadContent();
	PlayMusic();
}

void Scene1::LoadContent()
{
	SceneTag = Scene::SceneName::Scene1;
	//set time counter = 0
	mTimeCounter = 0;

	mPlayer = new Player();

	mPlayer->SetPosition(86, 1090);
	//mPlayer->SetPosition(462, 696);
	mPlayer->SetRevivePoint(GVector2(-1, -1));
	mPlayer->SetStartPoint(GVector2(50, 629));


	mMap = new GameMap("Resources/NewMap/Aladdin5.tmx");
	mMap->LoadMapItems("Resources/Scene1/ItemsInformation.txt");
	mMap->mPlayer = mPlayer;
	mMap->LoadMapObjects("Resources/Scene1/ObjectsInformation.txt");

	mCamera = new Camera(GameGlobal::GetWidth(), GameGlobal::GetHeight());
	mCamera->SetPosition(mCamera->GetWidth() / 2, mCamera->GetHeight() / 2);
	mMap->SetCamera(mCamera);


	mPlayer->SetCamera(mCamera);
	mPlayer->SetGameMap(mMap);

	CheckCameraAndWorldMap();;
	mSpriteHandler = GameGlobal::GetCurrentSpriteHandler();

	playerInfo = new PlayerInfo(mPlayer);
	playerInfo->SetCamera(mCamera);

}

void Scene1::Update(float dt)
{
	StopMusic();

	if (mPlayer->GetPosition().x > 4773)		//Điều kiện qua màn 1: Đi tới cuối màn.:>
	{
		Scene* t = new class EndScene(1, mPlayer);
		Sound::getInstance()->stop("Scene1");
		SceneManager::GetInstance()->ReplaceScene(t);
		return;
	}

	// cong don thoi gian lai 60 FPS = 1 / 60 giay trong 1 lan goi update
	if (mPlayer->getState() != PlayerState::Die && mPlayer->getState() != PlayerState::Revive) //Trạng thái không xét va chạm
		checkCollision(dt);
	else if (mPlayer->GetNumLives() < 0)				// Hết số mạng chuyển về màn hình EndScene:GameOver
	{
		Scene* t = new class EndScene(3, mPlayer);		// EndScene GameOver
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

void Scene1::Draw()
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
}

void Scene1::OnKeyDown(int keyCode)
{
	keys[keyCode] = true;
	mPlayer->OnKeyPressed(keyCode);
}

void Scene1::OnKeyUp(int keyCode)
{
	keys[keyCode] = false;
	mPlayer->OnKeyUp(keyCode);
}

void Scene1::OnMouseDown(float x, float y)
{}

void Scene1::CheckCameraAndWorldMap()			//Nếu Player ở các góc của màn hình thì set lại camera không cho ra phía ngoài của gamemap
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

void Scene1::checkCollision(float dt)
{
	mListApplesAladdin = mMap->GetListApples();
	mListEnemies = mMap->GetListEnemies();
	mListEnemyWeapons = mMap->GetListWeapons();

	unordered_set<Entity*> objectsCollision;

	playerCollideWith(objectsCollision, dt);
	appleCollideWith(objectsCollision);
	enemyWeaponCollideWith(objectsCollision);
}

void Scene1::enemyWeaponCollideWith(unordered_set<Entity*>& objectsCollision)
{
	for (auto enemiesWeapon : (*mListEnemyWeapons))
	{
		if (enemiesWeapon->IsDestroy())
			continue;

		objectsCollision.clear();
		mMap->GetGrid()->GetListEntity(objectsCollision, mCamera);

		for (auto objectCollide : objectsCollision)
		{
			auto r = GameCollision::rectCollide(enemiesWeapon->GetBound(), objectCollide->GetBound());

			if (r.IsCollided)
			{
				// lay phia va cham cua Entity so voi Player
				auto sideEnemyWeapon = GameCollision::getSideCollision(mPlayer, r);

				// lay phia va cham cua Player so voi Entity
				auto sideImpactor = GameCollision::getSideCollision(objectCollide, r);

				// goi den ham xu ly collision cua Apple va Entity
				enemiesWeapon->OnCollision(objectCollide, r, sideEnemyWeapon);
				objectCollide->OnCollision(enemiesWeapon, r, sideImpactor);
			}
		}
	}
}

void Scene1::appleCollideWith(unordered_set<Entity*>& objectsCollision)
{
	for (auto applesAladdin : (*mListApplesAladdin))
	{
		if (applesAladdin->IsDestroy() == true)
			continue;

		objectsCollision.clear();
		mMap->GetGrid()->GetListEntity(objectsCollision, mCamera);

#pragma region Apple collide with static object
		for (auto objectCollide : objectsCollision)
		{
			auto r = GameCollision::rectCollide(applesAladdin->GetBound(), objectCollide->GetBound());

			if (r.IsCollided)
			{
				// goi den ham xu ly collision cua Apple va Entity
				applesAladdin->OnCollision(objectCollide, r, r.sideCollision);
				objectCollide->OnCollision(applesAladdin, r, r.sideCollision);
			}
		}
#pragma endregion Apple collide with static object

#pragma region Apple collide with enemy
		for (auto enemies : (*mListEnemies))
		{
			if (enemies->IsDestroy() == true) continue;

			Entity::CollisionReturn r = GameCollision::rectCollide(applesAladdin->GetBound(),
				enemies->GetBound());

			if (r.IsCollided)
			{
				// goi den ham xu ly collision cua Apple va Entity
				// Đổi thứ tự xét Apple sau Enemies để Apple gây sát thương.
				enemies->OnCollision(applesAladdin, r, r.sideCollision);
				applesAladdin->OnCollision(enemies, r, r.sideCollision);
			}
		}
#pragma endregion Apple collide with enemy

#pragma region Apple collide with enemy weapon
		for (auto enemiesWeapon : (*mListEnemyWeapons))
		{
			if (enemiesWeapon->IsDestroy() == true)
				continue;

			auto r = GameCollision::rectCollide(applesAladdin->GetBound(),
				enemiesWeapon->GetBound());

			if (r.IsCollided)
			{
				// goi den ham xu ly collision cua Apple va Entity
				// Đổi thứ tự xét Apple sau Enemies để Apple gây sát thương.
				enemiesWeapon->OnCollision(applesAladdin, r, r.sideCollision);
				applesAladdin->OnCollision(enemiesWeapon, r, r.sideCollision);
			}
		}
#pragma endregion Apple collide with enemy weapon

	}
}

void Scene1::playerCollideWith(unordered_set<Entity*>& objectsCollision, float dt)
{
#pragma region PLAYER_COLLISION_DETECT

#pragma region PLAYER_WITH_STATIC_OBJECTS

	int widthBottom = 0;
	mMap->GetGrid()->GetListEntity(objectsCollision, mCamera);

	for (auto objectCollide : objectsCollision)
	{
		if (objectCollide->Tag == Entity::TYPE_BRICK)
		{
			if (((BrickDynamic*)objectCollide)->GetAnimation()->mCurrentIndex != 5
				&& ((BrickDynamic*)objectCollide)->GetAnimation()->mCurrentIndex != 4)
				continue;
		}

		auto r = GameCollision::rectCollide(mPlayer->GetBound(), objectCollide->GetBound());

		if (r.IsCollided)
		{
			// lay phia va cham cua Entity so voi Player
			auto sidePlayer = GameCollision::getSideCollision(mPlayer, r);

			// lay phia va cham cua Player so voi Entity
			auto sideImpactor = GameCollision::getSideCollision(objectCollide, r);

			// goi den ham xu ly collision cua Player va Entity
			mPlayer->OnCollision(objectCollide, r, sidePlayer);
			objectCollide->OnCollision(mPlayer, r, sideImpactor);

			if (objectCollide->Tag != Entity::HorizontalRope)

				// kiem tra neu va cham voi phia duoi cua Player 
				if (sidePlayer == Entity::Bottom
					|| sidePlayer == Entity::BottomLeft
					|| sidePlayer == Entity::BottomRight)
				{
					// kiem tra do dai  tiep xuc phia duoi day
					int bot = r.RegionCollision.right - r.RegionCollision.left;

					if (bot > widthBottom)
						widthBottom = bot;
				}
		}
	}

#pragma endregion


#pragma region PLAYER_WITH_ENEMIES_WEAPONS
	//Xét va chạm với vũ khí của Enemies.
	for (auto enemiesWeapon : (*mListEnemyWeapons))
	{
		if (enemiesWeapon->IsDestroy() == true)
			continue;

		auto r = GameCollision::rectCollide(mPlayer->GetBound(), enemiesWeapon->GetBound());

		if (r.IsCollided)
		{
			mPlayer->OnCollision(enemiesWeapon, r, r.sideCollision);
			enemiesWeapon->OnCollision(mPlayer, r, r.sideCollision);
		}
	}
#pragma endregion


#pragma region PLAYER_WITH_ENEMIES

	// Xét va chạm với Enemies
	for (auto enemies : (*mListEnemies))
	{
		if (enemies->IsDestroy() == true)
			continue;

		auto r = GameCollision::rectCollide(mPlayer->GetBound(), enemies->GetBound());

		if (r.IsCollided)
		{
			// lay phia va cham cua Player so voi Entity
			auto sideImpactor = GameCollision::getSideCollision(enemies, r);

			// Xử lý của Enemies và Player đều làm ở trong này.
			enemies->OnCollision(mPlayer, r, sideImpactor);

			if (enemies->Tag == Entity::FloatingGround 
				|| enemies->Tag == Entity::Camel 
				|| enemies->Tag == Entity::SpringBoard)
			{
				// lay phia va cham cua Entity so voi Player
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

	// Neu  dung ngoai mep thi luc nay cho Aladdin rot xuong duoi dat    
	if (widthBottom < Define::PLAYER_BOTTOM_RANGE_FALLING)
	{
		if (mPlayer->getState() != PlayerState::Climb)
			mPlayer->allowJump = false;
		mPlayer->OnNoCollisionWithBottom(dt);
	}
	else
		mPlayer->allowJump = true;

#pragma endregion

#pragma endregion
}


void Scene1::PlayMusic()	//Chạy nhạc nền.
{
	Sound::getInstance()->play("Scene1", true, 1);
}

void Scene1::StopMusic()	//Tắt nhạc nền
{
	Sound::getInstance()->stop("Scene1");
}

Scene::SceneName Scene1::GetSceneName()
{
	return Scene::SceneName::Scene1;
}