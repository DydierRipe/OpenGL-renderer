#include "observable.h"
#include "observer.h"

void Observable::notifyObservers(std::string eventID) {
    for (auto& observer : observers) {
        observer->onNotify(eventID);  // Llama al método onNotify() de cada observador
    }
}