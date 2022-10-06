cmake_minimum_required(VERSION 3.23)
project(comp345_project)

set(CMAKE_CXX_STANDARD 14)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config/001_I72_Ghtroc720.map DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config/004_I72_Cobra.map DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config/Antarctica.map DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config/Cornwall.map DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(comp345_project src/drivers/MapDriver.cpp src/map/Map.cpp include/Map.h src/drivers/MapLoader.cpp include/MapLoader.h "src/drivers/PlayerDriver.cpp" "src/Player.cpp" "include/Player.h")
