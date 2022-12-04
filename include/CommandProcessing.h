//
// COMP345_PROJECT_CommandProcessing_H CommandProcessing.h
//

#ifndef COMMANDPROCESSING_H
#define COMMANDPROCESSING_H

#include <string>
	using std::string;
#include <vector>
	using std::vector;
#include <ostream>
	using std::ostream;
#include <fstream>
	using std::fstream;

#include "LoggingObserver.h"

class State;
class Tournament;

// ---------------------------------------------
// -------------- Command Section --------------
// ---------------------------------------------
class Command: public ILoggable, public Subject {
public:
  Command();
  ~Command();
  Command(const Command &command);
  Command &operator=(const Command &command);
  void setCommand(string command);
  string getUserCommand();
  void saveEffect(string effect);
  std::string stringToLog();

private:
  string _command;
  string _effect;
  friend ostream &operator<<(ostream &strm, const Command &command);
};

// ---------------------------------------------
// ----------- FileLineReader Section ----------
// ---------------------------------------------

class FileLineReader {
public:
  FileLineReader();
  FileLineReader(const string filename);
  ~FileLineReader();
  FileLineReader(const FileLineReader &fileLineReader);
  FileLineReader &operator=(const FileLineReader &fileLineReader);

  bool readLineFromFile();
  string getCurrentLine();

private:
  string _filename;
  string _currentLine;
  fstream *_fStr;
  friend ostream &operator<<(ostream &strm,
                             const FileLineReader &fileLineReader);
};

// ---------------------------------------------
// ---------- CommandProcessor Section ---------
// ---------------------------------------------

class CommandProcessor: public ILoggable, public Subject {
public:
  CommandProcessor();
  ~CommandProcessor();
  CommandProcessor(const CommandProcessor &commandProcessor);
  CommandProcessor &operator=(const CommandProcessor &commandProcessor);

  string readCommand();
  Command *saveCommand(string command);
  Command *getCommand();
  bool validate(Command *command, int currentStateIndex, int &nextStateIndex,
                string &commandOption);
  vector<Command *> getCommandList();
  Command *getLastCommand();
  std::string stringToLog();
  void saveTournament(Tournament* t);
  Tournament* getTournament();


private:
  vector<State *> _stateList;
    bool validateTournamentCommand(const vector<string>&);
    Tournament* tournament;

protected:
  vector<Command *> _commandList;
  friend ostream &operator<<(ostream &strm,
                             const CommandProcessor &commandProcessor);
};

// ---------------------------------------------
// ---- FileCommandProcessorAdapter Section ----
// ---------------------------------------------

class FileCommandProcessorAdapter : public CommandProcessor {
public:
  FileCommandProcessorAdapter();
  FileCommandProcessorAdapter(string filename);
  ~FileCommandProcessorAdapter();
  FileCommandProcessorAdapter(
      const FileCommandProcessorAdapter &fileCommandProcessorAdapter);
  FileCommandProcessorAdapter &
  operator=(const FileCommandProcessorAdapter &fileCommandProcessorAdapter);

  string readCommand();
  Command *getCommand();

private:
  FileLineReader *_flr;
  friend ostream &
  operator<<(ostream &strm,
             const FileCommandProcessorAdapter &fileCommandProcessorAdapter);
};

#endif // COMP345_PROJECT_COMMANDPROCESSING_H
