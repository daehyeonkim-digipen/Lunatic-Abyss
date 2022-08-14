#include "../Engine/Engine.h"
#include "../Engine/Camera.h"
#include "../Engine/Graphic.h"
#include "../Engine/Texture.h"
#include "../Engine/TextureManager.h"
#include "Frontground.h"

void Frontground::add(const std::string& path, int levelX, int levelY) {
	frontgrounds.push_back({ Engine::get_GE_component<TextureManager>()->load(path), levelX, levelY });
}

void Frontground::clear() {
	for (ParallaxInfo& info : frontgrounds)
		Engine::get_GE_component<TextureManager>()->remove_texture(info.texturePtr);

	frontgrounds.clear();
}

void Frontground::draw(math::TransformMatrix displayMt) {

	for (ParallaxInfo& levelInfo : frontgrounds) {
		math::TransformMatrix layerMt = math::TranslateMatrix(-math::vec2{ levelInfo.levelX / 5, levelInfo.levelY / 3 });
		levelInfo.texturePtr->draw(displayMt * layerMt);
	}

}