//
// Created by Lujain Khalaf on 2022-11-29.
//

#include "../../include/GameEngine.h"
#include "../../include/CommandProcessing.h"

void testTournament()
{
    GameEngine* game = new GameEngine();
    cout << "Provide a tournament command:\n";
    CommandProcessor* cmdProcessor = game->getCommandProcessor();
    Command* tournamentCommand = cmdProcessor->getCommand();

    bool validCommand = false;
    string commandOption = "";
    validCommand = cmdProcessor->validate(
        tournamentCommand, game->getCurrentStateIndex(),
        game->getNextStateIndex(), commandOption);

    if (validCommand) {
        game->executeTournament(cmdProcessor->getTournament());
    }

    delete game;
}