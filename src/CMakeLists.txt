# src/CMakeLists.txt
# A few class libraries
add_library(Tile Tile.cpp)
add_library(skripler skripler.cpp)
add_library(textTools textTools.cpp)
add_library(Texture Texture.cpp)
add_library(Room Room.cpp)

# Define the executable
add_executable(maptrial main.cpp)

# Everyone get your headers from here
target_include_directories(maptrial PRIVATE ../include)
target_include_directories(skripler PRIVATE ../include)
target_include_directories(Tile PRIVATE ../include)
target_include_directories(textTools PRIVATE ../include)
target_include_directories(Texture PRIVATE ../include)
target_include_directories(Room PRIVATE ../include)

target_link_libraries(maptrial Tile)
target_link_libraries(maptrial Room)
target_link_libraries(maptrial skripler)


target_link_libraries(Tile textTools)
target_link_libraries(Tile Texture)

target_link_libraries(skripler Texture)
target_link_libraries(skripler Tile)
target_link_libraries(skripler textTools)
target_link_libraries(skripler pthread)

