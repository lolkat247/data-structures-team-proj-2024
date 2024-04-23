//
// Created by Jake Iglesias on 4/22/24.
//

#include "Airport.h"

Airport::Airport(const string &code, const string &cityState) {
    this->code = code;
    this->cityState = cityState;
}

string Airport::getState() const {
    size_t commaPos = cityState.find_last_of(',');
    if (commaPos != string::npos && commaPos + 2 < cityState.length()) {
        return cityState.substr(commaPos + 2);  // Two chars after the comma and space
    }

    return "";  // Return empty string if incorrect format; this shouldn't happen
}