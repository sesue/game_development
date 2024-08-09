#pragma once
#include "SpriteComponent.h"
#include <string>
#include <vector>

class TileBGComponent : public SpriteComponent
{
public:
	TileBGComponent(class Actor* owner, int drawOrder = 50);
	~TileBGComponent();

	void Draw(SDL_Renderer* renderer) override;
	void LoadTileCSV(const std::string& fileName, int tileWidth, int tileHeight);

	int mTileWidth = 0;
	int mTileHeight = 0;

private:
	std::vector<std::vector<int>> mLevelInts;
};