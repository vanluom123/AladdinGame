#pragma once

#include "GameCollision.h"
#include "Viewport.h"
#include "../GameObjects/Entity.h"

class QuadTree
{
public:

	QuadTree(int level, RECT bound);
	~QuadTree();

	void clear();
	void insertEntity(Entity* entity);

	void getEntitiesCollideAble(vector<Entity*>& entitiesOut, Entity* entity);
	void getAllEntities(vector<Entity*>& entitiesOut);
	int getTotalEntities();

	RECT getBounding() const { return _bounding; }
	vector<QuadTree*>* getNodes();

protected:

	vector<QuadTree*> _nodes;
	vector<Entity*> _listObject;

	int getIndex(RECT body);
	void split();
	bool _IsContain(Entity* entity);

	int _level;
	RECT _bounding;

};