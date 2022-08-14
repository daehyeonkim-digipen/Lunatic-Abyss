#pragma once
#include "../Engine/Component.h"
#include "../Engine/Math.h"

// this is same as background.h

class Camera;
class Texture;

class Frontground : public Component {
public:
    void add(const std::string& path, int levelX, int levelY);
    void clear();
    void draw(math::TransformMatrix displayMt);
private:
    bool hasSetted = false;

    struct ParallaxInfo {
        Texture* texturePtr;
        int levelX;
        int levelY;
    };
    std::vector<ParallaxInfo> frontgrounds;

};

