#include "../Scenes/Scene1.h"
#include "../GameObjects/Player/PlayerClimbingState.h"

#pragma warning(disable : 26812)

Scene1::Scene1()
{
	LoadContent();
	PlayMusic();
}

Scene1::~Scene1()
{
	GameMap::Release();

	delete mCamera;
	delete mPlayer;
	delete playerInfo;

	for (auto coll : mCollidable)
		delete coll;

	mCollidable.clear();
	keys.clear();
}

void Scene1::LoadContent()
{
	SceneTag = Scene::SceneName::Scene1;
	mTimeCounter = 0;

	mPlayer = new Player();
	mPlayer->SetPosition(86, 1090);
	//mPlayer->SetPosition(462, 696);
	mPlayer->SetRevivePoint(D3DXVECTOR2(-1, -1));
	mPlayer->SetStartPoint(D3DXVECTOR2(50, 629));

	GameMap::GetInstance()->Initialize("Resources/NewMap/Aladdin5.tmx");
	GameMap::GetInstance()->LoadMapItems("Resources/Scene1/ItemsInformation.txt");
	GameMap::GetInstance()->mPlayer = mPlayer;
	GameMap::GetInstance()->LoadMapObjects("Resources/Scene1/ObjectsInformation.txt");

	mCamera = new Camera(GameGlobal::GetWidth(), GameGlobal::GetHeight());
	mCamera->SetPosition(mCamera->GetWidth() / 2, mCamera->GetHeight() / 2);
	GameMap::GetInstance()->SetCamera(mCamera);

	mPlayer->SetCamera(mCamera);

	this->CheckCameraAndWorldMap();
	mSpriteHandler = GameGlobal::GetCurrentSpriteHandler();

	playerInfo = new PlayerInfo(mPlayer);
	playerInfo->SetCamera(mCamera);
}

void Scene1::Update(float dt)
{
	//Điều kiện qua màn 1: Đi tới cuối màn.:>
	if (mPlayer->GetPosition().x > 4773)
	{
		Scene* t = new class EndScene(1, mPlayer);
		Sound::getInstance()->stop("Scene1");
		SceneManager::GetInstance()->ReplaceScene(t);
		return;
	}

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
	this->CheckCameraAndWorldMap();
}

void Scene1::Draw()
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

void Scene1::CheckCameraAndWorldMap()
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

void Scene1::checkCollision(float dt)
{
	std::unordered_set<Entity*> listCollision;

	int widthBottom = 0;

	Grid::GetInstance()->GetListEntity(listCollision, mCamera);

	// Player with static object
	for (auto staticObj : listCollision)
	{
		auto r = GameCollision::RectBox(mPlayer->GetBound(), staticObj->GetBound());

		if (r.IsCollided)
		{
			// Side static object with player
			auto sidePlayer = GameCollision::GetSideCollision(mPlayer, r);

			// Side player with static object
			auto sideImpactor = GameCollision::GetSideCollision(staticObj, r);

			// Handle collision player with static object
			mPlayer->OnCollision(staticObj, r, sidePlayer);

			// Handle collision static object with player
			staticObj->OnCollision(mPlayer, r, sideImpactor);

			if (staticObj->Tag != Entity::HorizontalRope)
			{
				switch (sidePlayer)
				{
				case Entity::Bottom:
				case Entity::BottomLeft:
				case Entity::BottomRight:
				{
					int bot = r.RegionCollision.right - r.RegionCollision.left;

					if (bot > widthBottom)
						widthBottom = bot;

				}
				break;
				default:
					break;
				}
			}
		}
	}

	//Xét va chạm với vũ khí của Enemies.
	for (auto enemyWeapon : (*GameMap::GetInstance()->GetWeapons()))
	{
		if (enemyWeapon->IsDestroy() == true)
			continue;

		auto r = GameCollision::RectBox(mPlayer->GetBound(), enemyWeapon->GetBound());

		if (r.IsCollided)
		{
			mPlayer->OnCollision(enemyWeapon, r, r.sideCollision);
			enemyWeapon->OnCollision(mPlayer, r, r.sideCollision);
		}
	}

	// Xét va chạm với Enemies
	for (auto enemy : (*GameMap::GetInstance()->GetEnemies()))
	{
		if (enemy->IsDestroy() == true)
			continue;

		auto r = GameCollision::RectBox(mPlayer->GetBound(), enemy->GetBound());

		if (r.IsCollided)
		{
			//lay phia va cham cua Player so voi Entity
			auto sideImpactor = GameCollision::GetSideCollision(enemy, r);

			//Xử lý của Enemies và Player đều làm ở trong này.
			enemy->OnCollision(mPlayer, r, sideImpactor);

			switch (enemy->Tag)
			{
			case Entity::FloatingGround:
			case Entity::Camel:
			case Entity::SpringBoard:
			{
				//lay phia va cham cua Entity so voi Player
				auto sidePlayer = GameCollision::GetSideCollision(mPlayer, r);
				mPlayer->OnCollision(enemy, r, sidePlayer);

				switch (sidePlayer)
				{
				case Entity::Bottom:
				case Entity::BottomLeft:
				case Entity::BottomRight:
				{
					//kiem tra do dai  tiep xuc phia duoi day
					int bot = r.RegionCollision.right - r.RegionCollision.left;

					if (bot > widthBottom)
						widthBottom = bot;
				}
				break;
				default:
					break;
				}
			}
			break;
			default:
				break;
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
}

void Scene1::CheckCollisionEnemies(float dt)
{
	this->EnemiesWithStaticObject(dt);
}

void Scene1::EnemiesWithStaticObject(float dt)
{
	int width = 0;
	std::unordered_set<Entity*> objectsCollision;
	Grid::GetInstance()->GetListEntity(objectsCollision, mCamera);

	for (auto objColl : objectsCollision)
	{
		for (auto enemy : (*GameMap::GetInstance()->GetEnemies()))
		{
			if (enemy->IsDestroy())
				continue;

			auto r = GameCollision::RectBox(enemy->GetBound(), objColl->GetBound());

			if (r.IsCollided)
			{
				auto sideStaticEnemy = GameCollision::GetSideCollision(enemy, r);
				objColl->OnCollision(enemy, r, sideStaticEnemy);

				auto sideEnemyStatic = GameCollision::GetSideCollision(objColl, r);
				enemy->OnCollision(objColl, r, sideEnemyStatic);

				switch (sideEnemyStatic)
				{
				case Entity::Bottom:
				case Entity::BottomLeft:
				case Entity::BottomRight:
				{
					int bot = r.RegionCollision.right - r.RegionCollision.left;
					if (bot > width)
						width = bot;
				}
				break;

				default:
					break;
				}
			}
		}
	}


}


void Scene1::PlayMusic()
{
	Sound::getInstance()->play("Scene1", true, 1);
}

void Scene1::StopMusic()
{
	Sound::getInstance()->stop("Scene1");
}

Scene::SceneName Scene1::GetSceneName()
{
	return Scene::SceneName::Scene1;
}