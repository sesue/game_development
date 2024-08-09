#include "TileBGComponent.h"
#include "Actor.h"
#include "Game.h"
#include "SDL2/SDL.h"
#include "CSVHelper.h"
#include <string>
#include <istream>
#include <fstream>
#include <sstream>
#include <vector>

TileBGComponent::TileBGComponent(Actor* owner, int drawOrder)
: SpriteComponent(owner, drawOrder)
{
}

TileBGComponent::~TileBGComponent()
{
}

void TileBGComponent::Draw(SDL_Renderer* renderer)
{
	const int TILES_PER_ROW = GetTexWidth() / mTileWidth;

	for (int row = 0; row < mLevelInts.size(); row++)
	{
		for (int col = 0; col < mLevelInts[row].size(); col++)
		{
			if (mLevelInts[row][col] != -1)
			{
				// Establish draw space in window
				SDL_Rect r;
				r.w = mTileWidth;
				r.h = mTileHeight;
				r.x = (col * mTileWidth) - static_cast<int>(GetGame()->GetCameraPos().x);
				r.y = (row * mTileHeight) - static_cast<int>(GetGame()->GetCameraPos().y);

				// Establish tile to draw from tile map
				SDL_Rect srcRect;
				srcRect.w = mTileWidth;
				srcRect.h = mTileHeight;
				srcRect.x = (mLevelInts[row][col] % TILES_PER_ROW) * mTileWidth;
				srcRect.y = (mLevelInts[row][col] / TILES_PER_ROW) * mTileHeight;

				// Draw (have to convert angle from radians to degrees, and clockwise to counter)
				SDL_RenderCopyEx(renderer, mTexture, &srcRect, &r, 0.0, nullptr, SDL_FLIP_NONE);
			}
		}
	}
}

void TileBGComponent::LoadTileCSV(const std::string& fileName, int tileWidth, int tileHeight)
{
	mTileWidth = tileWidth;
	mTileHeight = tileHeight;

	//Load Level File
	std::ifstream file(fileName);

	std::string line;
	std::vector<int> row;
	while (getline(file, line))
	{
		std::vector<std::string> rowAsStr = CSVHelper::Split(line);
		row.clear();
		for (std::string str : rowAsStr)
			row.push_back(stoi(str));

		mLevelInts.push_back(row);
	}
}
