//
// COMP345_PROJECT_MAINDRIVER_CPP MainDriver.cpp
//
#include <cstring>
#include <iostream>
#include <string>
using std::string;

// Phase 1
void testLoadMaps();
void testPlayers();
void testOrdersList();
void testCards();
void testGameStates();

// Phase 2
void testStartupPhase(string fileName = "");
void testMainGameLoop();

int main(int argc, char *argv[]) {
  // Phase 1
  //    testLoadMaps();
  //    testPlayers();
  //    testOrdersList();
  //    testCards();
  //    testGameStates();

  // Phase 2
  if (argv[1] != NULL && (strcmp(argv[1], "-file") == 0)) {
    if (argv[2] != NULL) {
      string fileName = argv[2];
      testStartupPhase(fileName);
    } else {
      std::cout << "You did not enter the file name. Please rerun the program "
                   "and provide the file name";
    }
  } else if (argv[1] != NULL && (strcmp(argv[1], "-console") == 0)) {
    testStartupPhase();
  } else {
    std::cout << "Reading from console by default";
    testStartupPhase();
  }

  testMainGameLoop();

  return 0;
}