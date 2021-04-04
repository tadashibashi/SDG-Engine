#ifndef SDG_FRAME_H
#define SDG_FRAME_H
#include <Engine/Core.h>
#include <Engine/Content/Texture2D.h>

namespace SDG
{
    struct Frame
    {
        int x, y, w, h,
            ox, oy, ow, oh;
        Texture2D texture;
    };
}

#endif
