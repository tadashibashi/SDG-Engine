# Variables to use:
# FMOD_LIB_DIRS: for library directories
# FMOD_LIBS: for library names
# FMOD_INCLUDE_DIRS: for include directories

set (_FMODAPI "$ENV{FMODAPI}")
string(REPLACE "\\" "/" FMODAPI ${_FMODAPI})

if (CMAKE_SIZEOF_VOID_P MATCHES "4") #32-bit/DEBUG
    set(BUILD_TYPE "x86")
else()
    set(BUILD_TYPE "x64")
endif()

set(FMOD_LIB_DIRS
        "${FMODAPI}/core/lib/${BUILD_TYPE}"
        "${FMODAPI}/studio/lib/${BUILD_TYPE}")
set(FMOD_INCLUDE_DIRS ${FMODAPI}/studio/inc ${FMODAPI}/core/inc)

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    set(FMOD_LIBS fmodL_vc.lib fmodstudioL_vc.lib)
    file(COPY ${FMODAPI}/studio/lib/${BUILD_TYPE}/fmodstudioL.dll
            DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY ${FMODAPI}/core/lib/${BUILD_TYPE}/fmodL.dll
            DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
else()
    set(FMOD_LIBS fmod_vc.lib fmodstudio_vc.lib)
    file(COPY ${FMODAPI}/studio/lib/${BUILD_TYPE}/fmodstudio.dll
            DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY ${FMODAPI}/core/lib/${BUILD_TYPE}/fmod.dll
            DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()
