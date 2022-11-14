//
// Created by Lujain Khalaf on 2022-11-13.
//
#include "../include/LoggingObserver.h"
#include <fstream>

Observer::Observer() {}
Observer::Observer(const Observer& subject) {}
Observer::~Observer() {}

std::ostream& operator<<(std::ostream& out, const Observer& subject) {
    out << "Observer";
    return out;
}

Observer* Observer::operator=(const Observer& subject) {
    return this;
}

Subject::Subject() {}
Subject::Subject(const Subject& subject) {}
Subject::~Subject() {}

Subject* Subject::operator=(const Subject& subject) {
    return this;
}

std::ostream& operator<<(std::ostream& out, const Subject& subject) {
    out << "Subject";
    return out;
}


void Subject::notify()
{
    LogObserver::getInstance().update(dynamic_cast<ILoggable*>(this));
}

LogObserver::LogObserver() {
    std::fstream file("gamelog.txt", std::fstream::out);
    file << "";
    file.close();
}

LogObserver& LogObserver::getInstance() {
    static LogObserver instance;
    return instance;
}

LogObserver::LogObserver(const LogObserver& subject) {}
LogObserver::~LogObserver() {}

LogObserver* LogObserver::operator=(const LogObserver& subject) {
    return this;
}

std::ostream& operator<<(std::ostream& out, const LogObserver& subject) {
    out << "Log Observer";
    return out;
}

void LogObserver::outputToFile(std::string str)
{
    std::fstream file("gamelog.txt", std::fstream::app);
    file << str << "\n";
    file.close();
}
void LogObserver::update(ILoggable *loggable)
{
    outputToFile(loggable->stringToLog());
}

