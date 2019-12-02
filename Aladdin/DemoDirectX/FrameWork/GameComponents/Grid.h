#ifndef __GRID_H__
#define __GRID_H__

#include "../GameObjects/Entity.h"
#include "Camera.h"

#define GRID_ROW 52
#define GRID_COLUMN 38

#define CELL_HEIGHT 30
#define CELL_WIDTH 43
class Grid
{
private:
	unordered_set<Entity*> mcells[GRID_ROW][GRID_COLUMN];

public:
	Grid();
	~Grid();
	void InsertEntity(Entity* entity); // add entity vao grid
	void RemoveEntiy(Entity* entity);
	void GetListEntity(unordered_set<Entity*>& ListObj, Camera* camera);// lay nhung entiy nam trong viewport
	void GetCollisionableListEntity(unordered_set<Entity*>& ListObj, Entity* entity);//lay nhung entity co kha nang va cham voi player
};

#endif // !__GRID_H__
