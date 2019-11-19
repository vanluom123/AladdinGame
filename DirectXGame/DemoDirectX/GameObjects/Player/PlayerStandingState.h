#pragma once
#include "PlayerState.h"

class PlayerStandingState : public PlayerState
{
public:
    PlayerStandingState(PlayerData playerData);
    ~PlayerStandingState();

    void keyboardEventHandler(std::map<int, bool> keys) override;
    virtual StateName GetState();
};

