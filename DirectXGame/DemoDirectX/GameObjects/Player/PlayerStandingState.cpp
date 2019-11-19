#include "PlayerStandingState.h"
#include "Player.h"
#include "PlayerFallingState.h"
#include "../../GameDefines/GameDefine.h"

PlayerStandingState::PlayerStandingState(PlayerData playerData)
{
    _playerData = playerData;
	_playerData._player->SetVx(0);
	_playerData._player->SetVy(0);
}

PlayerStandingState::~PlayerStandingState()
{
}

void PlayerStandingState::keyboardEventHandler(std::map<int, bool> keys)
{
    if (keys[VK_LEFT] || keys[VK_RIGHT])
    {
        _playerData._player->setState(new PlayerRunningState(_playerData));
        return;
    }
}

PlayerState::StateName PlayerStandingState::GetState()
{
    return PlayerState::Standing;
}
