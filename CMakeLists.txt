cmake_minimum_required(VERSION 3.14)
project(Engine)

set(CMAKE_CXX_STANDARD 20)

#add_subdirectory(src/lua)
add_subdirectory(src/SDL)
add_subdirectory(src/tinyxml2)

#add_library(mojoAL STATIC src/mojoAL/mojoal.c)
#target_include_directories(mojoAL PUBLIC src/mojoAL $ENV{SDLBASE}/include)

# Get Architecture Bit Size
if (CMAKE_SIZEOF_VOID_P MATCHES "4")
    set(ARCH "32bit")
else()
    set(ARCH "64bit")
endif()

set(BUILD ${CMAKE_BUILD_TYPE})
message("Current Build Type: ${BUILD}")

set(AUDIO_ENGINE "")

if (EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "-s WASM=2 -Os")
    set(CMAKE_EXE_LINKER_FLAGS "-s FULL_ES3=1  -s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_ZLIB -s MAX_WEBGL_VERSION=2")
endif()

if (AUDIO_ENGINE MATCHES "WWISE")
    include(cmake/Wwise.cmake)
    set(AUDIO_INCLUDE_DIRS "")
    set(AUDIO_LIB_DIRS     "")
    set(AUDIO_LIBS         Wwise)
elseif(AUDIO_ENGINE MATCHES "FMOD")
    include(cmake/FMOD.cmake)
    set(AUDIO_INCLUDE_DIRS ${FMOD_INCLUDE_DIRS})
    set(AUDIO_LIB_DIRS     ${FMOD_LIB_DIRS})
    set(AUDIO_LIBS         ${FMOD_LIBS})
elseif(AUDIO_ENGINE MATCHES "MIXER")
    set(AUDIO_INCLUDE_DIRS "")
    set(AUDIO_LIB_DIRS     "")
    set(AUDIO_LIBS         "SDL_mixer")
else()
    set(AUDIO_INCLUDE_DIRS "")
    set(AUDIO_LIB_DIRS     "")
    set(AUDIO_LIBS         "")
endif()

# Engine
add_library(Engine STATIC
        src/Engine/Graphics/OpenGL/GraphicsDevice_GL.cpp
        src/Engine/Graphics/OpenGL/GraphicsDeviceMgr_GL.cpp
        src/Engine/Input/InputMgr.cpp
        src/Engine/Content/Shader.cpp
        src/Engine/Content/Shader.h
        src/Engine/Log.h
        src/Engine/File/IO.cpp
        src/Engine/File/ZipUtils.cpp
        src/Engine/File/PicoPNG.cpp 
        src/Engine/Input/Keyboard.cpp 
        src/Engine/Input/Keyboard.h 
        src/Engine/Graphics/DefaultShader.h 
        src/Engine/Graphics/SpriteBatch.cpp 
        src/Engine/Graphics/SpriteBatch.h 
        src/Engine/Content/ContentMgr.cpp 
        src/Engine/Content/ContentMgr.h 
        src/Engine/Content/TextureCache.cpp 
        src/Engine/Content/TextureCache.h 
        src/Engine/Content/Texture2D.h
        src/Engine/Graphics/Color.h 
        src/Engine/Graphics/Vertex.h 
        src/Engine/Graphics/Camera2D.h
        src/Engine/Graphics/Camera2D.cpp
        src/Engine/Input/Mouse.h
        src/Engine/Input/Mouse.cpp
        src/Engine/Input/Button.h
        src/Engine/Content/SpriteAtlas.h
        src/Engine/Content/SpriteAtlas.cpp
        src/Engine/Content/Frame.h
        src/Engine/Content/Sprite.h
        src/Engine/Content/Sprite.cpp
        src/Engine/Components/SpriteRenderer.cpp
        src/Engine/Components/SpriteRenderer.h
        src/Engine/Time/GameTime.cpp
        src/Engine/Time/GameTime.h
        src/Engine/Components/Component.h
        src/Engine/Components/Component.cpp
        src/Engine/Components/ComponentList.cpp
        src/Engine/Components/ComponentList.h
        src/Engine/Components/Transform.h
        src/Engine/Audio/Audio.cpp
        src/Engine/Audio/Audio.h
        src/Engine/Entity.cpp
        src/Engine/Entity.h
        src/Engine/EntityMgr.cpp
        src/Engine/EntityMgr.h
        src/Engine/Components/EntityComponent.cpp
        src/Engine/Components/EntityComponent.h
        src/Engine/Content/AtlasCache.cpp
        src/Engine/Content/AtlasCache.h
        src/Engine/Time/Chronogram.cpp
        src/Engine/Time/Chronogram.h
        src/Engine/Components/Body.h
        src/Engine/Time/Timer.cpp
        src/Engine/Time/Timer.h
        src/Engine/Components/Body.cpp
        src/Engine/Math/Math.h
        src/Engine/Scenes/Scene.h
        src/Engine/Game.cpp
        src/Engine/Game.h
        src/Engine/Scenes/SceneRunner.cpp
        src/Engine/Scenes/SceneRunner.h
        src/Engine/Scenes/SceneCache.cpp
        src/Engine/Scenes/SceneCache.h
        src/Engine/Scenes/SceneMgr.cpp
        src/Engine/Scenes/SceneMgr.h
        src/Engine/Scenes/Scene.cpp
        src/Engine/Math/Tween.cpp
        src/Engine/Time/Performance.cpp
        src/Engine/Audio/MixEngine/MixEngine.cpp
        src/Engine/Math/TweenFunctions.cpp
        src/Engine/File/SwapEndian.h
        src/Engine/GraphicsLibrary.h
        src/Engine/Content/Tiled/TileMap.h
        src/Engine/Content/Tiled/TileSet.h
        src/Engine/Content/Tiled/TileLayer.h
        src/Engine/Content/Tiled/Tile.h
        src/Engine/Content/Tiled/Property.h
        src/Engine/Content/Tiled/Object.h
        src/Engine/Content/Tiled/ObjectLayer.h
        src/Engine/Content/Tiled/Layer.h
        src/Engine/Content/Tiled/TileMap.cpp
        src/Engine/Content/Tiled/Property.cpp
        src/Engine/Graphics/Color.cpp
        src/Engine/Content/Tiled/Object.cpp
        src/Engine/Core.h
        src/Engine/EntryPoint.cpp
        src/Engine/Log.cpp
        src/Engine/Templates/Pool.h
        src/Engine/Math/Rand.cpp
        src/Engine/Math/Rand.h
        src/Engine/Graphics/Particles/Particle.h
        src/Engine/Graphics/Particles/ParticleEmitter.h
        src/Engine/Graphics/Particles/ParticleEmitter.cpp
        src/Engine/CollisionMgr.cpp
        src/Engine/CollisionMgr.h
        src/Engine/Components/Collider.h
        src/Engine/Components/Collider.cpp
        src/Engine/Scripting/VM.cpp
        src/Engine/Scripting/VM.h
        src/Engine/Graphics/SpriteFont.h
        src/Engine/Graphics/SpriteFont.cpp
        src/Engine/Events/Delegate.h
        src/Engine/States/StateMachine.h
        src/Engine/States/StateMachineState.h
        src/Engine/Graphics/GraphicsDeviceMgr.h
        src/Engine/Graphics/GraphicsDevice.h src/Engine/Platform.cpp src/Engine/Platform.h src/Engine/Graphics/OpenGL/SpriteBatch_GL.cpp src/Engine/Graphics/OpenGL/SpriteBatch_GL.h src/Engine/Content/Pixel.h src/Engine/Content/Frame.cpp)

target_include_directories(Engine PUBLIC
    src/
    src/lua
    src/SDL/include
    src/tinyxml2
    $ENV{SDL_TTF_BASE}
    C:/msvc/include/
    src/spdlog/include/
    ${AUDIO_INCLUDE_DIRS})

#if (WIN32 AND NOT MSVC)
#    message(FATAL_ERROR "On Windows, Engine only supports MSVC")
#endif()

# Engine Link Directories
if (ARCH MATCHES "32bit")       # 32-bit Builds
    if (BUILD MATCHES "Debug")
        message("=====Debug 32bit======")
        set(TARGET_LINK_DIRS ${TARGET_LINK_DIRS} $ENV{SDL_TTF_BASE}/VisualC/Win32/Debug C:/msvc/lib)
    else(BUILD MATCHES "Debug")
        message("=====Release 32bit======")
        set(TARGET_LINK_DIRS ${TARGET_LINK_DIRS} $ENV{SDL_TTF_BASE}/VisualC/Win32/Release C:/msvc/lib)
    endif(BUILD MATCHES "Debug")
elseif (ARCH MATCHES "64bit")   # 64-bit Builds
    if (BUILD MATCHES "Debug")
        message("=====Debug 64bit======")
        set(TARGET_LINK_DIRS ${TARGET_LINK_DIRS} $ENV{SDL_TTF_BASE}/VisualC/x64/Debug C:/msvc_x64/lib)
    else()
        message("=====Release 64bit======")
        set(TARGET_LINK_DIRS ${TARGET_LINK_DIRS} $ENV{SDL_TTF_BASE}/VisualC/x64/Release C:/msvc_x64/lib)
    endif()
else()
    message(FATAL "Unrecognized Build Settings")
endif()

target_link_directories(Engine PUBLIC ${TARGET_LINK_DIRS} ${AUDIO_LIB_DIRS})

# Engine Link Libraries
if (EMSCRIPTEN)
    set(LINK_LIBS tinyxml2)
    set(CMAKE_CXX_FLAGS "--preload-file ./assets@assets ${CMAKE_CXX_FLAGS}")
elseif(CMAKE_BUILD_TYPE MATCHES "Debug")
    set(LINK_LIBS SDL2main SDL2_mixer SDL2_ttf SDL2 opengl32 glew32d zlibstaticd tinyxml2)
else()
    set(LINK_LIBS SDL2main SDL2_mixer SDL2_ttf SDL2 opengl32 glew32 zlibstatic tinyxml2)
endif()

target_link_libraries(Engine ${LINK_LIBS} ${AUDIO_LIBS})

add_subdirectory(SurvivalGame)
add_subdirectory(Asteroids)
