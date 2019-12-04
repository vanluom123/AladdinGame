#include "Grid.h"

Grid::Grid()
{}

Grid::~Grid()
{}

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
			mcells[i][j].insert(entity);
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
			mcells[i][j].erase(mcells[i][j].find(entity));
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
			for (auto cell : mcells[i][j])
			{
				if (!cell->IsDestroy())
					ListObj.insert(cell);
			}
		}
	}
}

void Grid::GetCollisionableListEntity(unordered_set<Entity*>& ListObj, Entity* entity)
{
	// Do something
}
