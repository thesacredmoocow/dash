#pragma once

#include <obd/message.hpp>
#include "DashLog.hpp"

double percentage(Response resp) {
    return uint8_t(resp.raw_data.at(4)) - 48;
}

double temp(Response resp) {
    double raw_number = (uint8_t)resp.raw_data.at(1);
    return raw_number * 0.75 - 48.0;
    //eturn (uint8_t)resp.raw_data.at(1);
    //return (0.75*(uint8_t)resp.raw_data.at(1)) - 48.0; 
}

double rpm(Response resp) {
    //DASH_LOG(info)<<"rpm callback" << (int)resp.raw_data.at(3) << " " << (int)resp.raw_data.at(2);
    return ((256.0 * (uint8_t)resp.raw_data.at(3)) + (int)resp.raw_data.at(2)) / 6.4; 
}

double speed(Response resp) {
     return ((256.0 * (int)resp.raw_data.at(2)) + (int)resp.raw_data.at(1)) / 120; 
}

double flow(Response resp) { return ((256.0 * (int)resp.data.at(0)) + (int)resp.data.at(1)) / 100.0; }

