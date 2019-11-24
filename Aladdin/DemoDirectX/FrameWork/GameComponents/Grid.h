#include "../GameObjects/Entity.h"
#include "Camera.h"

#define GRID_ROW 52
#define GRID_COLUMN 38

#define CELL_HEIGHT 30
#define CELL_WIDTH 43

class Grid
{
private:
	Grid();
	static Grid* _instance;

	unordered_set<Entity*> _objects;
	unordered_set<Entity*> _cells[GRID_ROW][GRID_COLUMN];

public:
	~Grid();
	static Grid* GetInstance();
	static void Release();

	// add entity vao grid
	void InsertEntity(Entity* entity);
	void RemoveEntiy(Entity* entity);

	// lay nhung entiy nam trong viewport
	void GetListEntity(unordered_set<Entity*>& ListObj, Camera* camera);

	//lay nhung entity co kha nang va cham voi player
	void GetPossibleObjectCollide(unordered_set<Entity*>& ListObj, Entity* entity);
	
};
