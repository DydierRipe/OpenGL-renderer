#pragma once
#include <string>

class Observer {
public:
    virtual void onNotify(std::string eventID) = 0;  // M�todo que ser� llamado cuando el observable cambie
};
