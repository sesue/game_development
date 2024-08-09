#include "HeightMap.h"
#include "CSVHelper.h"
#include "SDL2/SDL.h"

#include <fstream>
#include <algorithm>
#include <string>

HeightMap::HeightMap()
{
	//Load Level File
	std::ifstream file(FILE_PATH);

	std::string line;
	int i = 0;
	while (getline(file, line))
	{
		std::vector<std::string> row = CSVHelper::Split(line);
		mHeightGrid.push_back({});
		for (std::string data : row)
			mHeightGrid[i].push_back(stoi(data));
		i++;
	}
}

HeightMap::~HeightMap()
{
}

bool HeightMap::IsCellOnTrack(int row, int col)
{
	return mHeightGrid[row][col] != -1;
}

float HeightMap::GetHeightFromCell(int row, int col)
{
	return -40.0f + static_cast<float>(mHeightGrid[row][col]) * 5.0f;
}

Vector3 HeightMap::CellToWorld(int row, int col)
{
	if (!IsCellOnTrack(row, col))
		return Vector3::Zero;

	Vector3 position;
	position.x = GRID_TOP - CELL_SIZE * row;
	position.y = GRID_LEFT + CELL_SIZE * col;
	position.z = GetHeightFromCell(row, col);

	return position;
}

Vector2 HeightMap::WorldToCell(const Vector2& pos)
{
	int row = (Math::Abs(pos.x - GRID_TOP) + CELL_SIZE / 2.0f) / CELL_SIZE;
	int col = (Math::Abs(pos.y - GRID_LEFT) + CELL_SIZE / 2.0f) / CELL_SIZE;

	return Vector2(row, col);
}

bool HeightMap::IsOnTrack(const Vector2& pos)
{
	Vector2 cell = WorldToCell(pos);
	return IsCellOnTrack(cell.x, cell.y);
}

float HeightMap::GetHeight(const Vector2& pos)
{
	Vector2 cell = WorldToCell(pos);
	return GetHeightFromCell(cell.x, cell.y);
}