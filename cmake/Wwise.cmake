project(Wwise)

# WWISE
# Library Dependencies
SET (Wwise_LIBRARIES AkSoundEngine AkMemoryMgr AkStreamMgr AkMusicEngine AkSpatialAudio CommunicationCentral)
IF (WIN32)
    SET (Wwise_NATIVE_DEPENDENCIES dinput8 Winmm dsound dxguid ws2_32 xinput)
ELSEIF (UNIX)
    SET (Wwise_NATIVE_DEPENDENCIES "")
ENDIF ()

set (_WWISESDK $ENV{WWISESDK})
string(REPLACE "\\" "/" WWISESDK ${_WWISESDK})

set (WWISE_SRC_FILES
        "${WWISESDK}/samples/SoundEngine/Common/AkDefaultLowLevelIODispatcher.cpp "
        "${WWISESDK}/samples/SoundEngine/Common/AkFileLocationBase.cpp "
        "${WWISESDK}/samples/SoundEngine/Common/AkFilePackage.cpp "
        "${WWISESDK}/samples/SoundEngine/Common/AkFilePackageLowLevelIO.inl "
        "${WWISESDK}/samples/SoundEngine/Common/AkFilePackageLUT.cpp "
        "${WWISESDK}/samples/SoundEngine/Common/AkMultipleFileLocation.cpp "
        "${WWISESDK}/samples/SoundEngine/Win32/AkDefaultIOHookBlocking.cpp "
        "${WWISESDK}/samples/SoundEngine/Win32/AkDefaultIOHookDeferred.cpp "
        "${WWISESDK}/samples/SoundEngine/Win32/stdafx.cpp")
message("Build type is: ${CMAKE_BUILD_TYPE}")
if (CMAKE_SIZEOF_VOID_P MATCHES "4" AND CMAKE_BUILD_TYPE MATCHES "Debug") #32-bit/DEBUG
    set(WWISE_LIB_DIR "${WWISESDK}/Win32_vc160/Debug(StaticCRT)/lib")
elseif(CMAKE_SIZEOF_VOID_P MATCHES "4" AND CMAKE_BUILD_TYPE MATCHES "Release")                           # Release
    set(WWISE_LIB_DIR "${WWISESDK}/Win32_vc160/Release(StaticCRT)/lib")
elseif(CMAKE_SIZEOF_VOID_P MATCHES "8" AND CMAKE_BUILD_TYPE MATCHES "Debug")
    set(WWISE_LIB_DIR "${WWISESDK}/x64_vc160/Debug(StaticCRT)/lib")
elseif(CMAKE_SIZEOF_VOID_P MATCHES "8" AND CMAKE_BUILD_TYPE MATCHES "Release")
    set(WWISE_LIB_DIR "${WWISESDK}/x64_vc160/Release(StaticCRT)/lib")
endif()

add_library(Wwise)
target_sources(Wwise PRIVATE ${WWISE_SRC_FILES})
target_include_directories(Wwise PUBLIC "${WWISESDK}/include" "${WWISESDK}/samples/SoundEngine/Common"
        "${WWISESDK}/samples/SoundEngine/Win32")
target_link_directories(Wwise PUBLIC "${WWISE_LIB_DIR}")
target_link_libraries(Wwise PUBLIC "${Wwise_LIBRARIES}" "${Wwise_NATIVE_DEPENDENCIES}")