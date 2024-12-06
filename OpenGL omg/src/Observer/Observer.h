#pragma once
#include <string>

class Observer {
public:
    virtual void onNotify(std::string eventID) = 0;  // Método que será llamado cuando el observable cambie
};
