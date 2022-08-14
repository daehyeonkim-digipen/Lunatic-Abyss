#include "Engine.h"
#include "Texture.h"
#include "TextureManager.h"


Texture* TextureManager::load(const std::string& filePath) {
    if (pathToTexture.find(filePath) == pathToTexture.end()) {
        pathToTexture[filePath] = new Texture(filePath, texUnitIDIndex);
        doesAllocated[texUnitIDIndex] = true;

        while(doesAllocated[texUnitIDIndex]) {
            texUnitIDIndex = (++texUnitIDIndex) % 128;
        }
    }
    return pathToTexture[filePath];
}

TextureManager::~TextureManager() {
    for (std::pair<std::string, Texture*> pathTexturePair : pathToTexture) {
        delete pathTexturePair.second;
    }
    pathToTexture.clear();
}

void TextureManager::remove_texture(Texture* SpriteTexture) {
    if (SpriteTexture == nullptr) {
        Engine::get_logger().log_debug("NULL delete detected in texture");
        return;
    }

    std::string target;
    for (auto textures : pathToTexture)
    {
        if (textures.second == SpriteTexture)
        {
            doesAllocated[SpriteTexture->unitID] = false;
            delete SpriteTexture;
            SpriteTexture = nullptr;
            target = textures.first;
        }
    }

    pathToTexture.erase(target);
}