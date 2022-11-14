cmake_minimum_required(VERSION 3.23)
project(comp345_project)

set(CMAKE_CXX_STANDARD 14)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config/001_I72_Ghtroc720.map DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config/004_I72_Cobra.map DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config/Antarctica.map DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config/APKomp.map DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config/Cornwall.map DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config/duplicate_continent.map DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config/duplicate_territory.map DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config/missing_continent.map DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config/missing_territory.map DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(
        comp345_project
        src/drivers/MapDriver.cpp src/Map.cpp include/Map.h
        src/drivers/PlayerDriver.cpp src/Player.cpp include/Player.h
        src/drivers/OrderDriver.cpp src/Orders.cpp include/Orders.h
        src/drivers/GameEngineDriver.cpp src/GameEngine.cpp include/GameEngine.h
        src/drivers/MainDriver.cpp
        include/Cards.h src/Cards.cpp src/drivers/CardsDriver.cpp include/LoggingObserver.h src/LoggingObserver.cpp src/drivers/LoggingObserverDriver.cpp)
