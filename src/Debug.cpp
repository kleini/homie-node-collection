#include "Debug.hpp"
#include "Homie.hpp"

void Debug::debugSend(uint16_t id) {
    Homie.getLogger() << "MQTT packet send " << id << endl;
};
