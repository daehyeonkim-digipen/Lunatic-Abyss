#include "../Engine/Engine.h"
#include "../Engine/Texture.h"
#include "../Engine/TextureManager.h"
#include "../Engine/Scene.h"
#include "../Engine/Game.h"
#include "../Engine/Camera.h"
#include "../Engine/showCollision.h"
#include "../Engine/GameObjectManager.h"

#include "CollisionBlock.h"
#include "GameMap.h"
#include <vector>

GameMap::GameMap(const std::string& filePath, const std::string& blockFileName, const std::string& mapFileName) {
	blockTexture = Engine::get_GE_component<TextureManager>()->load(filePath + blockFileName);
	mapTexture = Engine::get_GE_component<TextureManager>()->load(filePath + mapFileName);
	
	mapSize = mapTexture->get_size();
	math::vec2 block_map_size = blockTexture->get_size();

	std::vector<math::rect2> blockAreas;

	auto isPixelCoordInAreas = [&](math::vec2 pixelCoord)->bool {
		for (const math::rect2& area : blockAreas)
			if (area.point1.x <= pixelCoord.x && pixelCoord.x <= area.point2.x
				&& area.point1.y <= pixelCoord.y && pixelCoord.y <= area.point2.y)
				return true;
		return false;
	};

	auto GOM = Engine::get_GE_component<Game>()->get_game_component<GameObjectManager>();

	unsigned int checkColor = 0xffff00ff;

	for (int y = 0; y < block_map_size.y; y++) {
		for (int x = 0; x < block_map_size.x; x++) {
			math::vec2 position{ x, y };
			if (blockTexture->get_pixel(x, y) == checkColor) {
				if (isPixelCoordInAreas(position)) continue;

				int pixelCheckCoord = y + 1;
				int height = 1, width = 0;
				while (blockTexture->get_pixel(x, pixelCheckCoord) == checkColor) {
					height++;
					pixelCheckCoord++;
				}

				pixelCheckCoord = x + 1;
				while (blockTexture->get_pixel(pixelCheckCoord, y + (int)height - 1) == checkColor
					&& blockTexture->get_pixel(pixelCheckCoord, y) == checkColor
					&& blockTexture->get_pixel(pixelCheckCoord, y + (int)height) == 0x0) {
					if (isPixelCoordInAreas({ pixelCheckCoord, y }))
						break;

					width++;
					pixelCheckCoord++;
				}

				blockAreas.push_back(math::rect2{ position, position + math::vec2{ width, height } });
				GOM->add_game_object(ObjType::Block, new CollisionBlock(position, math::vec2{ width, height }));
			}
				
		}
	}
}

GameMap::~GameMap() {
	Engine::get_GE_component<TextureManager>()->remove_texture(mapTexture);
}

void GameMap::draw() {
	Camera* cam = Engine::get_GE_component<Game>()->get_game_component<Camera>();
		
	mapTexture->draw(cam->get_matrix());
}

math::vec2 GameMap::get_stage_size() {
	return mapSize;
}
