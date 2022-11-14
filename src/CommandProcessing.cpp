
//
// COMP345_PROJECT_GAMEENGINE_CPP CommandProcessing.cpp
//

#include <string>
using std::string;
#include <vector>
using std::vector;

#include <fstream>
using std::ifstream;
#include <sstream>
using std::ios;
using std::stringstream;
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include "../include/CommandProcessing.h"

// Holds the state names to be used across class
static const string stateName[]{"start",
                                "mapLoaded",
                                "mapValidated",
                                "playersAdded",
                                "assignReinforcement",
                                "issueOrders",
                                "executeOrders",
                                "win"};

// Trim is used to remove the empty spaces at the beginning and the end of a
// string
string trim(const string &originalString) {
  size_t firstCharNotASpace = originalString.find_first_not_of("\t\f\v\n\r ");
  string noLeadingSpaces = (firstCharNotASpace == string::npos)
                               ? ""
                               : originalString.substr(firstCharNotASpace);

  size_t lastCharNotASpace = noLeadingSpaces.find_last_not_of(" \t\f\v\n\r");
  return (firstCharNotASpace == string::npos)
             ? ""
             : noLeadingSpaces.substr(0, lastCharNotASpace + 1);
  ;
}

// ---------------------------------------------
// -------------- Command Section --------------
// ---------------------------------------------

Command::Command() {
  _command = "";
  _effect = "";
}
Command::~Command() {}

Command::Command(const Command &command) {
  _command = command._command;
  _effect = command._effect;
}

Command &Command::operator=(const Command &command) {
  _command = command._command;
  _effect = command._effect;
  return *this;
}

ostream &operator<<(ostream &strm, const Command &command) {
  return strm << "The current command is \"" + command._command +
                     "\" and its effect is \"" + command._effect + "\"";
}

void Command::saveEffect(string effect) { _effect = effect; }
void Command::setCommand(string command) { _command = command; }
string Command::getUserCommand() { return _command; }

// ---------------------------------------------
// ----------- FileLineReader Section ----------
// ---------------------------------------------

FileLineReader::FileLineReader() {}

FileLineReader::FileLineReader(const string filename) {
  _filename = filename;
  _currentLine = "";
  _fStr = new fstream();
  ;
  _fStr->open(filename, ios::in);
  if (!_fStr->is_open()) {
    cout << "File does not exist!" << endl;
    exit(0);
  }
}

FileLineReader::~FileLineReader() {
  _fStr->close();
  delete _fStr;
  _fStr = nullptr;
}

FileLineReader::FileLineReader(const FileLineReader &fileLineReader) {
  _filename = fileLineReader._filename;
  _currentLine = fileLineReader._currentLine;
  _fStr = fileLineReader._fStr;
}

FileLineReader &
FileLineReader::operator=(const FileLineReader &fileLineReader) {
  _filename = fileLineReader._filename;
  _currentLine = fileLineReader._currentLine;
  _fStr = fileLineReader._fStr;
  return *this;
}

ostream &operator<<(ostream &strm, const FileLineReader &fileLineReader) {
  return strm << "The current file is \"" + fileLineReader._filename + "\"";
}

bool FileLineReader::readLineFromFile() {
  string currentLineNotTrimmed;
  if (_fStr->eof()) {
    return false;
  } else {
    getline(*_fStr, currentLineNotTrimmed);
    _currentLine = trim(currentLineNotTrimmed);
    return true;
  }
  return false;
}
string FileLineReader::getCurrentLine() { return _currentLine; }

// ---------------------------------------------
// ---------- CommandProcessor Section ---------
// ---------------------------------------------

CommandProcessor::CommandProcessor() {
  _stateList = {
      new State(stateName[0], {new Transition("loadmap", 1)}),
      new State(stateName[1], {new Transition("loadmap", 1),
                               new Transition("validatemap", 2)}),
      new State(stateName[2], {new Transition("addplayer", 3)}),
      new State(stateName[3], {new Transition("addplayer", 3),
                               new Transition("gamestart", 4)}),
      new State(stateName[4], {new Transition("issueorder", 5)}),
      new State(stateName[5], {new Transition("issueorder", 5),
                               new Transition("endissueorders", 6)}),
      new State(stateName[6],
                {new Transition("execorder", 6),
                 new Transition("endexecorders", 4), new Transition("win", 7)}),
      new State(stateName[7],
                {new Transition("play", 0), new Transition("quit", -1)}),
  };
  vector<Command *> _commandList;
}

CommandProcessor::~CommandProcessor() {
  for (Command *command : _commandList) {
    delete command;
    command = nullptr;
  }
  _commandList.clear();

  for (State *state : _stateList) {
    delete state;
    state = nullptr;
  }
  _stateList.clear();
}

CommandProcessor::CommandProcessor(const CommandProcessor &commandProcessor) {
  _commandList = vector<Command *>();
  for (auto const &command : commandProcessor._commandList) {
    _commandList.push_back(new Command(*command));
  }
  _stateList = commandProcessor._stateList;
}

CommandProcessor &
CommandProcessor::operator=(const CommandProcessor &commandProcessor) {
  _commandList = commandProcessor._commandList;
  _stateList = commandProcessor._stateList;
  return *this;
}

ostream &operator<<(ostream &strm, const CommandProcessor &commandProcessor) {
  for (auto const &command : commandProcessor._commandList) {
    strm << *command << endl;
  }
  return strm;
}

string CommandProcessor::readCommand() {
  string userCommand;
  getline(cin, userCommand);
  string userCommandTrimmed = trim(userCommand);
  return userCommandTrimmed;
}

Command *CommandProcessor::saveCommand(string userCommand) {
  Command *command = new Command();
  command->setCommand(userCommand);
  _commandList.push_back(command);
  return _commandList.back();
}

Command *CommandProcessor::getCommand() {
  string userCommand = readCommand();
  Command *command = saveCommand(userCommand);
  return command;
}

bool CommandProcessor::validate(Command *command, int currentStateIndex,
                                int &nextStateIndex, string &commandOption) {
  bool validCommand = false;

  string userCommand = command->getUserCommand();
  stringstream ss(userCommand);
  string commandText;
  ss >> commandText; // get first token of input string
  if (commandText == "loadmap") {
    ss >> commandOption;
    if (commandOption == "") {
      cout << "You did not enter the map file name." << endl;
      command->saveEffect("User did not enter the map file name.");
      return false;
    }
    userCommand = commandText;
  } else if (commandText == "addplayer") {
    ss >> commandOption;
    if (commandOption == "") {
      cout << "You did not enter the player name." << endl;
      command->saveEffect("User did not enter the player name.");
      return false;
    }
    userCommand = commandText;
  }
  // Check the user command against the valid commands at the current state
  //  and set the current state index to the next state.
  for (auto const &transition :
       _stateList[currentStateIndex]->getTransition()) {
    if (userCommand == transition->getCommand()) {
      nextStateIndex = transition->getNextStateIndex();
      validCommand = true;
    }
  }
  return validCommand;
}

vector<Command *> CommandProcessor::getCommandList() { return _commandList; }

Command *CommandProcessor::getLastCommand() { return _commandList.back(); }

// ---------------------------------------------
// ---- FileCommandProcessorAdapter Section ----
// ---------------------------------------------

FileCommandProcessorAdapter::FileCommandProcessorAdapter()
    : CommandProcessor() {
  FileLineReader *_flr;
}
FileCommandProcessorAdapter::FileCommandProcessorAdapter(string filename)
    : CommandProcessor() {
  _flr = new FileLineReader(filename);
}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
  delete _flr;
  _flr = nullptr;
}

FileCommandProcessorAdapter::FileCommandProcessorAdapter(
    const FileCommandProcessorAdapter &fileCommandProcessorAdapter)
    : CommandProcessor(fileCommandProcessorAdapter) {
  _flr = new FileLineReader(*fileCommandProcessorAdapter._flr);
}

FileCommandProcessorAdapter &FileCommandProcessorAdapter::operator=(
    const FileCommandProcessorAdapter &fileCommandProcessorAdapter) {
  CommandProcessor::operator=(fileCommandProcessorAdapter);
  _flr = fileCommandProcessorAdapter._flr;
  return *this;
}

string FileCommandProcessorAdapter::readCommand() {
  if (_flr->readLineFromFile()) {
    string userCommand = _flr->getCurrentLine();
    cout << "The command from the file is \"" + userCommand + "\"" << endl;

    return userCommand;
  } else {
    cout << "End of file. There are no more commands" << endl;
    exit(0);
  }
}

ostream &
operator<<(ostream &strm,
           const FileCommandProcessorAdapter &fileCommandProcessorAdapter) {
  strm << "FileCommandProcessorAdapter \n";
  strm << *fileCommandProcessorAdapter._flr;

  for (auto const &command : fileCommandProcessorAdapter._commandList) {
    strm << *command << endl;
  }
  return strm;
}
Command *FileCommandProcessorAdapter::getCommand() {
  string userCommand = readCommand();
  Command *command = saveCommand(userCommand);
  return command;
}
