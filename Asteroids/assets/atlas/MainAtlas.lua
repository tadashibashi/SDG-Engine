--
-- created with TexturePacker - https://www.codeandweb.com/texturepacker
--
-- $TexturePacker:SmartUpdate:e512eaf2ae7d77abdd787ee0afc5af7b:5245a8ff8ef201f2275af91e97ff26da:c796868e79b8923eb0f5c3c5e448e722$
--
-- local sheetInfo = require("mysheet")
-- local myImageSheet = graphics.newImageSheet( "mysheet.png", sheetInfo:getSheet() )
-- local sprite = display.newSprite( myImageSheet , {frames={sheetInfo:getFrameIndex("sprite")}} )
--

local SheetInfo = {}

SheetInfo.sheet =
{
    frames = {
    
        {
            -- asteroid_large1
            x=0,
            y=0,
            width=32,
            height=32,

        },
        {
            -- asteroid_large2
            x=0,
            y=32,
            width=32,
            height=32,

        },
        {
            -- asteroid_large3
            x=0,
            y=64,
            width=32,
            height=32,

        },
        {
            -- asteroid_med1
            x=38,
            y=0,
            width=16,
            height=20,

            sourceX = 8,
            sourceY = 5,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- asteroid_med2
            x=0,
            y=96,
            width=20,
            height=20,

            sourceX = 6,
            sourceY = 6,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- asteroid_med3
            x=20,
            y=96,
            width=18,
            height=20,

            sourceX = 6,
            sourceY = 7,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- asteroid_small1
            x=10,
            y=116,
            width=10,
            height=10,

            sourceX = 11,
            sourceY = 11,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- asteroid_small2
            x=32,
            y=20,
            width=12,
            height=14,

            sourceX = 9,
            sourceY = 8,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- asteroid_small3
            x=0,
            y=116,
            width=10,
            height=12,

            sourceX = 11,
            sourceY = 10,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- ship
            x=20,
            y=116,
            width=8,
            height=8,

        },
    },

    sheetContentWidth = 128,
    sheetContentHeight = 128
}

SheetInfo.frameIndex =
{

    ["asteroid_large1"] = 1,
    ["asteroid_large2"] = 2,
    ["asteroid_large3"] = 3,
    ["asteroid_med1"] = 4,
    ["asteroid_med2"] = 5,
    ["asteroid_med3"] = 6,
    ["asteroid_small1"] = 7,
    ["asteroid_small2"] = 8,
    ["asteroid_small3"] = 9,
    ["ship"] = 10,
}

function SheetInfo:getSheet()
    return self.sheet;
end

function SheetInfo:getFrameIndex(name)
    return self.frameIndex[name];
end

return SheetInfo
