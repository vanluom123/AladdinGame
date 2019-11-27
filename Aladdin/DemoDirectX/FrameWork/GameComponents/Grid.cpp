#include "Grid.h"

Grid::Grid()
{
}

Grid::~Grid()
{

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
			mcells[i][j].push_back(entity);
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
			for (int k = 0; k < mcells[i][j].size(); k++)
			{
				if (mcells[i][j].at(k) == entity)
				{
					mcells[i][j].erase(mcells[i][j].begin() + k);
				}

			}

		}
	}
}

void Grid::GetListEntity(vector<Entity*>& ListObj, Camera* camera)
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
			for (UINT k = 0; k < mcells[i][j].size(); k++)
			{
				if (mcells[i][j].at(k)->IsDestroy() == false)
				{
					ListObj.push_back(mcells[i][j].at(k));
				}
			}
		}
	}
}

void Grid::GetCollisionableListEntity(vector<Entity*>& ListObj, Entity* entity)
{
}
