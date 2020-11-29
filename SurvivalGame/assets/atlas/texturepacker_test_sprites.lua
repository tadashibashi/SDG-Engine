sprites = {
    {
        key = "rob-idle",
        frames = {
            "rob/1",
            "rob/2",
            "rob/3",
            "rob/4"
        },
        reel = { 1, 2, 3, 4 },   -- '1' indexing
        offset = {               -- normalized
            x = 0.5,
            y = 1
        },
        mask = {
            x = 8,
            y = 16,
            w = 16,
            h = 16
        },
        base_speed = 4
    },
    {
        key = "guy-idle",
        frames = {
            "guy/1",
            "guy/2",
            "guy/3",
            "guy/4"
        },
        reel = { 1, 2, 3, 4 },
        offset = {
            x = .5,
            y = 1
        },
        mask = {
            x = 8,
            y = 16,
            w = 16,
            h = 16
        },
        base_speed = 4
    },
    {
        key = "compy-idle",
        frames = {
            "compy/1",
            "compy/2",
            "compy/3",
            "compy/4",
            "compy/5"
        },
        reel = { 1, 2, 3, 4, 5 },
        offset = {
            x = .5,
            y = 1
        },
        mask = {
            x = 8,
            y = 16,
            w = 16,
            h = 16
        },
        base_speed = 6
    }
}


