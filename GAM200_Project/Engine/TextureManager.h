#pragma once
#include "Component.h"

#include <map>
#include <string>

class Texture;
class TextureManager : public Component {
public:
    Texture* load(const std::string& filePath);
    ~TextureManager();
    void remove_texture(Texture*);
private:
    std::map<std::string, Texture*> pathToTexture;

    unsigned int texUnitIDIndex = 0;
    bool doesAllocated[128] = { false };
};