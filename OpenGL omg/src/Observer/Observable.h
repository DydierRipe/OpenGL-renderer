#pragma once

#include <vector>
#include <memory>
#include <string>

class Observer;

class Observable {
public:
    void addObserver(std::shared_ptr<Observer> observer) {
        observers.push_back(observer);
    }

    void notifyObservers(std::string eventID);

protected:
    std::vector<std::shared_ptr<Observer>> observers;
};
