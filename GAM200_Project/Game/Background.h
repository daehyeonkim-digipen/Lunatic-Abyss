#pragma once
#include "../Engine/Component.h"
#include "../Engine/Math.h"

class Camera;
class Texture;

class Background : public Component {
public:
	void add(const std::string& path, int levelX, int levelY);
	void clear();
	void draw(math::TransformMatrix displayMt);
private:
    struct ParallaxInfo {
        Texture* texturePtr;
        int levelX;
        int levelY;
    };
    std::vector<ParallaxInfo> backgrounds;
};

