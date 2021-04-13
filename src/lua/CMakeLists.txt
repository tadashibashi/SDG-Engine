project(lua)
cmake_minimum_required(VERSION 3.15)

include_directories(.)

set(LUA_SRC lapi.c lcode.c lctype.c ldebug.c ldo.c ldump.c lfunc.c lgc.c llex.c lmem.c lobject.c lopcodes.c
        lparser.c lstate.c lstring.c ltable.c ltm.c lundump.c lvm.c lzio.c ltests.c lauxlib.c lbaselib.c
        ldblib.c liolib.c lmathlib.c loslib.c ltablib.c lstrlib.c lutf8lib.c loadlib.c lcorolib.c linit.c
)

set(CMAKE_CXX_FLAGS "-Os")

if (DEBUG)
    add_library(luad STATIC ${LUA_SRC})
else()
    add_library(lua STATIC ${LUA_SRC})
endif()
