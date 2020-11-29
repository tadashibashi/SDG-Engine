--
-- created with TexturePacker - https://www.codeandweb.com/texturepacker
--
-- $TexturePacker:SmartUpdate:28c11a46912be8e02af849eee887af55:89e0a105db1105b2ad6640789867fbe7:ed26315ae7682e4964bc798d091a9d29$
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
            -- balloon/1
            x=1,
            y=1,
            width=16,
            height=30,

            sourceX = 8,
            sourceY = 2,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- balloon/2
            x=1,
            y=33,
            width=16,
            height=30,

            sourceX = 8,
            sourceY = 1,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- bug/1
            x=23,
            y=133,
            width=20,
            height=20,

            sourceX = 7,
            sourceY = 12,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- bug/2
            x=45,
            y=133,
            width=20,
            height=20,

            sourceX = 7,
            sourceY = 12,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- bug/3
            x=47,
            y=111,
            width=20,
            height=20,

            sourceX = 7,
            sourceY = 12,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- bug/4
            x=1,
            y=195,
            width=20,
            height=22,

            sourceX = 7,
            sourceY = 10,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- bug/5
            x=1,
            y=219,
            width=20,
            height=22,

            sourceX = 7,
            sourceY = 10,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- compy/1
            x=1,
            y=93,
            width=20,
            height=24,

            sourceX = 5,
            sourceY = 8,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- compy/2
            x=19,
            y=1,
            width=22,
            height=24,

            sourceX = 6,
            sourceY = 8,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- compy/3
            x=1,
            y=119,
            width=20,
            height=24,

            sourceX = 7,
            sourceY = 8,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- compy/4
            x=1,
            y=145,
            width=20,
            height=24,

            sourceX = 6,
            sourceY = 8,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- compy/5
            x=1,
            y=65,
            width=20,
            height=26,

            sourceX = 6,
            sourceY = 6,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- crab/1
            x=23,
            y=155,
            width=22,
            height=14,

            sourceX = 5,
            sourceY = 18,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- crab/2
            x=69,
            y=117,
            width=22,
            height=14,

            sourceX = 5,
            sourceY = 18,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- crab/3
            x=85,
            y=43,
            width=20,
            height=14,

            sourceX = 7,
            sourceY = 18,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- crab/4
            x=107,
            y=1,
            width=18,
            height=14,

            sourceX = 6,
            sourceY = 18,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- crab/5
            x=89,
            y=21,
            width=20,
            height=14,

            sourceX = 7,
            sourceY = 18,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- frog/1
            x=1,
            y=243,
            width=16,
            height=12,

            sourceX = 8,
            sourceY = 20,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- frog/2
            x=71,
            y=67,
            width=16,
            height=18,

            sourceX = 8,
            sourceY = 6,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- frog/3
            x=47,
            y=175,
            width=16,
            height=16,

            sourceX = 8,
            sourceY = 7,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- frog/4
            x=19,
            y=53,
            width=18,
            height=10,

            sourceX = 7,
            sourceY = 22,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- frog/5
            x=89,
            y=59,
            width=16,
            height=14,

            sourceX = 8,
            sourceY = 18,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- ghost/1
            x=67,
            y=23,
            width=20,
            height=18,

            sourceX = 6,
            sourceY = 8,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- ghost/2
            x=85,
            y=1,
            width=20,
            height=18,

            sourceX = 6,
            sourceY = 8,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- ghost/3
            x=43,
            y=193,
            width=20,
            height=18,

            sourceX = 6,
            sourceY = 8,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- ghost/4
            x=47,
            y=155,
            width=20,
            height=18,

            sourceX = 6,
            sourceY = 8,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- ghost/5
            x=67,
            y=133,
            width=20,
            height=18,

            sourceX = 6,
            sourceY = 6,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- guy/1
            x=23,
            y=195,
            width=18,
            height=22,

            sourceX = 7,
            sourceY = 10,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- guy/2
            x=1,
            y=171,
            width=22,
            height=22,

            sourceX = 4,
            sourceY = 10,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- guy/3
            x=23,
            y=219,
            width=18,
            height=22,

            sourceX = 7,
            sourceY = 10,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- guy/4
            x=23,
            y=65,
            width=16,
            height=22,

            sourceX = 8,
            sourceY = 10,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- guy/5
            x=19,
            y=27,
            width=22,
            height=24,

            sourceX = 4,
            sourceY = 8,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- owl/1
            x=67,
            y=1,
            width=16,
            height=20,

            sourceX = 8,
            sourceY = 12,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- owl/2
            x=25,
            y=171,
            width=20,
            height=20,

            sourceX = 5,
            sourceY = 6,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- owl/3
            x=23,
            y=89,
            width=24,
            height=20,

            sourceX = 3,
            sourceY = 5,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- owl/4
            x=65,
            y=45,
            width=18,
            height=20,

            sourceX = 7,
            sourceY = 4,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- owl/5
            x=49,
            y=75,
            width=20,
            height=20,

            sourceX = 4,
            sourceY = 12,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- rob/1
            x=69,
            y=97,
            width=20,
            height=18,

            sourceX = 6,
            sourceY = 14,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- rob/2
            x=41,
            y=53,
            width=22,
            height=20,

            sourceX = 5,
            sourceY = 12,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- rob/3
            x=43,
            y=1,
            width=22,
            height=20,

            sourceX = 5,
            sourceY = 12,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- rob/4
            x=43,
            y=23,
            width=22,
            height=20,

            sourceX = 4,
            sourceY = 12,
            sourceWidth = 32,
            sourceHeight = 32
        },
        {
            -- rob/5
            x=23,
            y=111,
            width=22,
            height=20,

            sourceX = 5,
            sourceY = 11,
            sourceWidth = 32,
            sourceHeight = 32
        },
    },

    sheetContentWidth = 256,
    sheetContentHeight = 256
}

SheetInfo.frameIndex =
{

    ["balloon/1"] = 1,
    ["balloon/2"] = 2,
    ["bug/1"] = 3,
    ["bug/2"] = 4,
    ["bug/3"] = 5,
    ["bug/4"] = 6,
    ["bug/5"] = 7,
    ["compy/1"] = 8,
    ["compy/2"] = 9,
    ["compy/3"] = 10,
    ["compy/4"] = 11,
    ["compy/5"] = 12,
    ["crab/1"] = 13,
    ["crab/2"] = 14,
    ["crab/3"] = 15,
    ["crab/4"] = 16,
    ["crab/5"] = 17,
    ["frog/1"] = 18,
    ["frog/2"] = 19,
    ["frog/3"] = 20,
    ["frog/4"] = 21,
    ["frog/5"] = 22,
    ["ghost/1"] = 23,
    ["ghost/2"] = 24,
    ["ghost/3"] = 25,
    ["ghost/4"] = 26,
    ["ghost/5"] = 27,
    ["guy/1"] = 28,
    ["guy/2"] = 29,
    ["guy/3"] = 30,
    ["guy/4"] = 31,
    ["guy/5"] = 32,
    ["owl/1"] = 33,
    ["owl/2"] = 34,
    ["owl/3"] = 35,
    ["owl/4"] = 36,
    ["owl/5"] = 37,
    ["rob/1"] = 38,
    ["rob/2"] = 39,
    ["rob/3"] = 40,
    ["rob/4"] = 41,
    ["rob/5"] = 42,
}

function SheetInfo:getSheet()
    return self.sheet;
end

function SheetInfo:getFrameIndex(name)
    return self.frameIndex[name];
end

return SheetInfo
