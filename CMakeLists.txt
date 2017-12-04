cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(SnowSimulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

set(ATLAS_DIR "${PROJECT_SOURCE_DIR}/lib/atlas")
set(ATLAS_CONFIG_DIR "${ATLAS_DIR}/config")
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/source")
set(SHADER_DIR "${PROJECT_SOURCE_DIR}/shaders")
set(ASSET_DIR "${PROJECT_SOURCE_DIR}/assets")

# Sets a path to code that will be automatically generated
set(GENERATED_DIR "${PROJECT_SOURCE_DIR}/generated")

add_subdirectory(${ATLAS_DIR})
add_subdirectory(${INCLUDE_DIR})
add_subdirectory(${SOURCE_DIR})
add_subdirectory(${SHADER_DIR})
add_subdirectory(${ASSET_DIR})

include_directories(${ATLAS_INCLUDE_DIRS})
include_directories(${INCLUDE_DIR})
include_directories(${GENERATED_DIR})

link_libraries(${APP_NAME} ${ATLAS_LIBRARIES})

source_group(include FILES ${INCLUDE_FILES})
source_group(source FILES ${SOURCE_FILES})
source_group(shaders FILES ${SHADER_FILES})

add_executable(snowSimulation ${SOURCE_FILES} ${INCLUDE_FILES} ${SHADER_FILES})
