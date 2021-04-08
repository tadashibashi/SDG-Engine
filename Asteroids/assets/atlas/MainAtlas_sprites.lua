sprites = {
    {
        key = "ship",
        frames = {
            "ship"
        },
        reel = { 1 },   -- '1' indexing
        offset = {               -- normalized
            x = .3,
            y = .5
        },
        mask = {
            x = 1,
            y = 2,
            w = 4,
            h = 4
        },
        base_speed = 0
    },
    {
        key = "asteroid-small",
        frames = {
            "asteroid_small1",
            "asteroid_small2",
            "asteroid_small3",
        },
        reel = { 1, 2, 3 },   -- '1' indexing
        offset = {            -- normalized
            x = 0.5,
            y = 0.5
        },
        mask = {
            x = 12,
            y = 12,
            w = 8,
            h = 8
        },
        base_speed = 0
    },
    {
        key = "asteroid-med",
        frames = {
            "asteroid_med1",
            "asteroid_med2",
            "asteroid_med3",
        },
        reel = { 1, 2, 3 },   -- '1' indexing
        offset = {            -- normalized
            x = 0.5,
            y = 0.5
        },
        mask = {
            x = 8,
            y = 8,
            w = 16,
            h = 16
        },
        base_speed = 0
    },
    {
        key = "asteroid-large",
        frames = {
            "asteroid_large1",
            "asteroid_large2",
            "asteroid_large3",
        },
        reel = { 1, 2, 3 },   -- '1' indexing
        offset = {            -- normalized
            x = .5,
            y = .5
        },
        mask = {
            x = 8,
            y = 8,
            w = 24,
            h = 24
        },
        base_speed = 0
    }
}
