//
// Created by Lujain Khalaf on 2022-11-29.
//

#include "../../include/GameEngine.h"
#include "../../include/CommandProcessing.h"

void testTournament()
{
    GameEngine* game = new GameEngine();
    cout << "Provide a tournament command:\n";
    CommandProcessor* cmdProcessor= new CommandProcessor();
    cmdProcessor->getCommand();
}