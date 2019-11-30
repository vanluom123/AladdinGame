#pragma once

class Bat;
class BatState;

typedef struct BatData
{
	Bat* batEnemy;
	BatState* state;
}BATDATA, *LPBATDATA;