#pragma once

class BatEnemy;
class BatEnemyState;

class BatEnemyData
{
public:
	BatEnemyData();
	
	~BatEnemyData();

	BatEnemy *batEnemy;
	BatEnemyState *state;
private:

};

