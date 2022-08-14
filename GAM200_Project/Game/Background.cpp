#include "../Engine/Engine.h"
#include "../Engine/Camera.h"
#include "../Engine/Graphic.h"
#include "../Engine/Texture.h"
#include "../Engine/TextureManager.h"
#include "Background.h"

void Background::add(const std::string& path, int levelX, int levelY) {
	backgrounds.push_back({ Engine::get_GE_component<TextureManager>()->load(path), levelX, levelY });
}

void Background::clear() {
    for (ParallaxInfo& info : backgrounds)
        Engine::get_GE_component<TextureManager>()->remove_texture(info.texturePtr);

    backgrounds.clear();
}

void Background::draw(math::TransformMatrix displayMt) {
    for (ParallaxInfo& levelInfo : backgrounds) {
        math::TransformMatrix layerMt = math::TranslateMatrix(-math::vec2{ levelInfo.levelX / 5, levelInfo.levelY / 3 });
        levelInfo.texturePtr->draw(displayMt * layerMt);
    }
}