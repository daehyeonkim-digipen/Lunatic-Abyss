#include "../Engine/Texture.h"
#include "../Engine/TextureManager.h"

#include "Cinematic.h"

Cinematic::Cinematic() {
	blackLineTex = Engine::get_GE_component<TextureManager>()->load("assets/UI/cinematic_line.png");
}

Cinematic::~Cinematic() {
	Engine::get_GE_component<TextureManager>()->remove_texture(blackLineTex);
}

void Cinematic::update(double dt) {
	if (0 < playTime) {
		if (lineTimer < lineHeight) {
			lineTimer += dt * lineSpeedOffset;
		}

		playTime -= dt;
	}
	else {
		if (!is_playing() && 0 <= lineTimer) {
			lineTimer -= dt * lineSpeedOffset;
		}

		isCinematic = false;
	}
}

void Cinematic::play(double time) {
	playTime = time;
	isCinematic = true;
}

void Cinematic::stop() {
	playTime = 0;
	isCinematic = false;
};

void Cinematic::draw() {
	if (lineTimer < 0) return;

	math::vec2 winSize = Engine::get_GE_component<IGL::Graphic>()->get_window_size();
	math::vec2 texSize = blackLineTex->get_size();

	math::vec2 offset = { winSize.x / texSize.x, lineHeight / texSize.y };

	blackLineTex->draw(math::TranslateMatrix{ { 0.0, -lineHeight + lineTimer } } * math::ScaleMatrix{ offset });
	blackLineTex->draw(math::TranslateMatrix{ { 0.0, winSize.y - lineTimer } } * math::ScaleMatrix{ offset });
}


