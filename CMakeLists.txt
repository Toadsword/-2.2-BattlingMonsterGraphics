cmake_minimum_required(VERSION 3.9)
project(BattlingMonsters)

file(GLOB_RECURSE BattlingMonsters_SRC src/*.cpp include/*.h include/*.hpp)

include_directories(include/)
add_executable(BattlingMonsters ${BattlingMonsters_SRC})
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/externals)



#SFML
set(SFML_DIR ${EXTERNAL_DIR}/SFML)
set(SFML_BUILD_EXAMPLES FALSE)
add_subdirectory(${SFML_DIR})
include_directories(${SFML_DIR}/include)

LIST(APPEND BattlingMonsters_LIBRARIES
	sfml-system
	sfml-window
	sfml-graphics
	sfml-audio
	sfml-network)
	
target_link_libraries(BattlingMonsters ${BattlingMonsters_LIBRARIES})
	