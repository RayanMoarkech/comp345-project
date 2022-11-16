//
// Created by Lujain Khalaf on 2022-11-13.
//
#ifndef COMP345_PROJECT_LOGGINGOBSERVER_H
#define COMP345_PROJECT_LOGGINGOBSERVER_H

#pragma once

#include <string>
#include <iostream>

class ILoggable {
public:
    //All classes that implement ILoggable must implement this stringToLog method
    virtual std::string stringToLog() = 0;
};

class Observer {
public:
    Observer();
    Observer(const Observer& subject);
    ~Observer();
    Observer* operator=(const Observer& subject);
    friend std::ostream& operator<<(std::ostream& out, const Observer& subject);
    //Calls stringToLog on the subject and writes it to the log file
    virtual void update(ILoggable*) = 0;
};

class Subject {
public:
    Subject();
    Subject(const Subject& subject);
    Subject* operator=(const Subject& subject);
    friend std::ostream& operator<<(std::ostream& out, const Subject& subject);
    virtual ~Subject();

//    virtual void attach(Observer*);
//    virtual void detach(Observer*);

    virtual void notify();
};

class LogObserver: public Observer {

public:
    LogObserver();
    LogObserver(const LogObserver& subject);
    LogObserver* operator=(const LogObserver& subject);
    ~LogObserver();
    void update(ILoggable*);
    static LogObserver& getInstance();
    friend std::ostream& operator<<(std::ostream& out, const LogObserver& subject);

private:
    void outputToFile(std::string str);
};

#endif //COMP345_PROJECT_LOGGINGOBSERVER_H
