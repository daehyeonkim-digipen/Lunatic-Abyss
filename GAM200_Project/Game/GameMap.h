#pragma once
#include "../Engine/Component.h"
#include "../Engine/Math.h"
#include "../Engine/Scene.h"
#include <vector>

class GameObject;
class GameMap : public Component {
public:
	GameMap(const std::string& filePath, const std::string& blockFileName, const std::string& mapFileName);
	~GameMap() override;

	void draw();
	math::vec2 get_stage_size();
private:
	Texture* blockTexture;
	Texture* mapTexture;
	math::vec2 mapSize;
};

