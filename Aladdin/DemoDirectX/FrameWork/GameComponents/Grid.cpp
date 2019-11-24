#include "Grid.h"

Grid* Grid::_instance = nullptr;


Grid::Grid()
{}

Grid::~Grid()
{}

Grid* Grid::GetInstance()
{
	if (_instance == nullptr)
		_instance = new Grid();
	return _instance;
}

void Grid::Release()
{
	delete _instance;
	_instance = nullptr;
}

void Grid::InsertEntity(Entity* entity)
{
	if (entity == NULL)
		return;

	auto bound = entity->GetBound();
	int startX = floor(bound.left / CELL_WIDTH);
	int endX = floor(bound.right / CELL_WIDTH);
	int startY = floor(bound.top / CELL_HEIGHT);
	int endY = floor(bound.bottom / CELL_HEIGHT);

	for (int i = startY; i <= endY; i++)
	{
		for (int j = startX; j <= endX; j++)
		{
			_cells[i][j].insert(entity);
		}
	}
}

void Grid::RemoveEntiy(Entity* entity)
{
	if (entity == NULL)
		return;
	auto bound = entity->GetBound();
	int startX = floor(bound.left / CELL_WIDTH);
	int endX = floor(bound.right / CELL_WIDTH);
	int startY = floor(bound.top / CELL_HEIGHT);
	int endY = floor(bound.bottom / CELL_HEIGHT);

	for (int i = startY; i <= endY; i++)
	{
		for (int j = startX; j <= endX; j++)
		{
			for (auto it : _cells[i][j])
			{
				if (it == entity)
					_cells[i][j].erase(it);
			}
		}
	}
}

void Grid::GetListEntity(unordered_set<Entity*>& ListObj, Camera* camera)
{
	ListObj.clear();

	int startX = floor(camera->GetBound().left / CELL_WIDTH);
	int endX = floor(camera->GetBound().right / CELL_WIDTH);
	int startY = floor(camera->GetBound().top / CELL_HEIGHT);
	int endY = floor(camera->GetBound().bottom / CELL_HEIGHT);

	for (int i = startY; i <= endY; i++)
	{
		for (int j = startX; j <= endX; j++)
		{
			for (auto it : _cells[i][j])
			{
				if (it->IsDestroy() == false)
					ListObj.insert(it);
			}
		}
	}
}

void Grid::GetPossibleObjectCollide(unordered_set<Entity*>& ListObj, Entity* entity)
{
}
