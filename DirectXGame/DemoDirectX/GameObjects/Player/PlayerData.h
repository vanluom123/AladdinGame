#ifndef __PLAYER_DATA_H
#define __PLAYER_DATA_H

class Player;
class PlayerState;

struct PlayerData
{
	Player* _player;
	PlayerState* _pState;
};

#endif // __PLAYER_DATA_H