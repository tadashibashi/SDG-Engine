project(SurvivalGame)

add_executable(SurvivalGame
        SurvivalGame.cpp Player.h Zombie.h GameScene.h Bullet.h)

target_link_libraries(SurvivalGame Engine)

if (WIN32)
    target_compile_definitions(SurvivalGame PRIVATE SDG_PLATFORM_WINDOWS)
endif()

# Copy symlink of assets folder into binary directory.
if (NOT IS_SYMLINK "${CMAKE_CURRENT_BINARY_DIR}/assets")
    set(source "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    set(destination "${CMAKE_CURRENT_BINARY_DIR}/assets")
    add_custom_command(
            TARGET SurvivalGame PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination}
            DEPENDS ${destination}
            COMMENT "symbolic link resources folder from ${source} => ${destination}"
    )
endif()
