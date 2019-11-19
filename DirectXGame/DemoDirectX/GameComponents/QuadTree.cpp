#include "QuadTree.h"
#include "GameGlobal.h"

QuadTree::~QuadTree()
{}

QuadTree::QuadTree(int level, RECT bound)
{
	this->_bounding = bound;
	this->_level = level;
}

void QuadTree::clear()
{
	if (_nodes.empty() == false)
	{
		for (auto node : _nodes)
		{
			node->clear();
			delete node;
			node = nullptr;
		}

		_nodes.clear();
	}
}

void QuadTree::insertEntity(Entity* entity)
{
	int index = getIndex(entity->GetBounding());

	if (_nodes.empty() == false)
	{
		if (index != -1)
		{
			_nodes.at(index)->insertEntity(entity);
			return;
		}
	}

	if (index == -1)
		this->_listObject.push_back(entity);
	else
	{
		if (_nodes.empty() == true)
			this->split();

		_nodes.at(index)->insertEntity(entity);
	}
}

bool QuadTree::_IsContain(Entity* entity)
{
	RECT r = entity->GetBounding();
	return (r.left >= getBounding().left && r.right <= getBounding().right && r.top >= getBounding().top && r.bottom <= getBounding().bottom);
}

void QuadTree::split()
{
	RECT bound;

	int width = (getBounding().right - getBounding().left) / 2;
	int height = (getBounding().bottom - getBounding().top) / 2;

	bound.left = getBounding().left;
	bound.right = bound.left + width;
	bound.top = getBounding().top;
	bound.bottom = bound.top + height;
	_nodes.push_back(new QuadTree(_level + 1, bound));

	bound.left = getBounding().left + width;
	bound.right = bound.left + width;
	bound.top = getBounding().top;
	bound.bottom = bound.top + height;
	_nodes.push_back(new QuadTree(_level + 1, bound));

	bound.left = getBounding().left;
	bound.right = bound.left + width;
	bound.top = getBounding().top + height;
	bound.bottom = bound.top + height;
	_nodes.push_back(new QuadTree(_level + 1, bound));

	bound.left = getBounding().left + width;
	bound.right = bound.left + width;
	bound.top = getBounding().top + height;
	bound.bottom = bound.top + height;
	_nodes.push_back(new QuadTree(_level + 1, bound));

}

int QuadTree::getTotalEntities()
{
	int total = _listObject.size();

	if (_nodes.empty() == false)
	{
		for (auto node : _nodes)
			total += node->getTotalEntities();
	}

	return total;
}

vector<QuadTree*>* QuadTree::getNodes()
{
	return &_nodes;
}

int QuadTree::getIndex(RECT body)
{
	float middleVerticle = getBounding().left + (getBounding().right - getBounding().left) / 2.0f;
	float middleHorizontal = getBounding().top + (getBounding().bottom - getBounding().top) / 2.0f;

	if (body.top > getBounding().top&& body.bottom < middleHorizontal)
	{
		if (body.left > getBounding().left&& body.right < middleVerticle)
		{
			return 0;
		}
		else if (body.left > middleVerticle&& body.right < getBounding().right)
		{
			return 1;
		}
	}
	else if (body.top > middleHorizontal&& body.bottom < getBounding().bottom)
	{
		if (body.left > getBounding().left&& body.right < middleVerticle)
		{
			return 2;
		}
		else if (body.left > middleVerticle&& body.right < getBounding().right)
		{
			return 3;
		}
	}

	return -1;
}

void QuadTree::getAllEntities(std::vector<Entity*>& entitiesOut)
{
	for (auto child : _listObject)
		entitiesOut.push_back(child);

	if (_nodes.empty() == false)
	{
		for (auto node : _nodes)
			node->getAllEntities(entitiesOut);
	}
}

void QuadTree::getEntitiesCollideAble(std::vector<Entity*>& entitiesOut, Entity* entity)
{
	int index = this->getIndex(entity->GetBounding());

	if (index != -1)
	{
		for (auto child : _listObject)
			entitiesOut.push_back(child);

		if (_nodes.empty() == false)
		{
			_nodes.at(index)->getEntitiesCollideAble(entitiesOut, entity);
		}
	}
	else
		this->getAllEntities(entitiesOut);
}