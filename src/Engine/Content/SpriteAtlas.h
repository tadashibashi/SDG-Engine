
#ifndef SDG_SPRITEATLAS_H
#define SDG_SPRITEATLAS_H
#include <Engine/Core.h>
#include <map>
#include <string>
#include "Frame.h"
#include "Texture2D.h"
#include "Sprite.h"

namespace SDG
{
    class ContentMgr;
    class SpriteAtlas
    {
        friend class AtlasCache;
    public:
        explicit SpriteAtlas(ContentMgr &content);
        ~SpriteAtlas();

        // No copying.
        SpriteAtlas(const SpriteAtlas &) = delete;
        SpriteAtlas &operator=(const SpriteAtlas &) = delete;

        bool Load(const std::string &atlasPath, const std::string &imagePath);
        [[nodiscard]] Frame GetFrame(const std::string &key) const;
        bool LoadSprites(const std::string &spriteConfigPath);
        void LogFrames();

        Sprite *GetSprite(const std::string &key);

        void Close();
    private:
        ContentMgr &content_;

        std::map<std::string, Frame> frames_;
        std::map<std::string, Sprite *> sprites_;
        std::string imagePath_;
        std::string atlasPath_;
        Texture2D texture_;
    };
}

#endif
