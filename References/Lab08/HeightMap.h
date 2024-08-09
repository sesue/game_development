#pragma once
#include <vector>
#include <string>
#include <SDL2/SDL_stdinc.h>
#include "Math.h"

class HeightMap
{
public:
	HeightMap();
	~HeightMap();

	Vector3 CellToWorld(int row, int col);
	Vector2 WorldToCell(const Vector2& pos);
	bool IsOnTrack(const Vector2& pos);
	float GetHeight(const Vector2& pos);

	const std::string FILE_PATH = "Assets/HeightMap/HeightMap.csv";

	const float CELL_SIZE = 40.55f;
	const float GRID_TOP = 1280.0f;
	const float GRID_LEFT = -1641.0f;

private:
	bool IsCellOnTrack(int row, int col);
	float GetHeightFromCell(int row, int col);

	std::vector<std::vector<int>> mHeightGrid;
};
