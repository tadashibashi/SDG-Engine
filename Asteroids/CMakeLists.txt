project(Asteroids)

add_executable(${PROJECT_NAME} Asteroids.cpp Ship.h Asteroid.h Ship.cpp Scenes/GameScene.cpp Bullet.cpp)

target_link_libraries(${PROJECT_NAME} Engine)

if (WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE SDG_PLATFORM_WINDOWS)
endif()

# Copy symlink of assets folder into binary directory.
if (NOT IS_SYMLINK "${CMAKE_CURRENT_BINARY_DIR}/assets")
    set(source "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    set(destination "${CMAKE_CURRENT_BINARY_DIR}/assets")
    add_custom_command(
            TARGET ${PROJECT_NAME} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination}
            DEPENDS ${destination}
            COMMENT "symbolic link resources folder from ${source} => ${destination}"
    )
endif()

if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.html)
    execute_process(COMMAND ${CMAKE_COMMAND}
            -E copy ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.html ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.html)
endif()
