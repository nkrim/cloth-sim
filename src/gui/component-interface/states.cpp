/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: states.cpp
 *  Definition file for States class
 * **************************************************************************** */

#include "states.h"

// constructor
States::States()
    : _states{0}
{ }
States::States(unsigned states)
    : _states{states}
{ }

// mutators
void States::setState(StateBit state, bool value) {
    setStates((unsigned)state, value);
}
void States::setStates(unsigned states, bool value) {
    if(value)
        _states |= states;
    else 
        _states &= ~states;
}
void States::setAllStates(unsigned states) {
    _states = states;
}
void States::toggleState(StateBit state) {
    _states ^= state;
}
void States::toggleStates(unsigned states) {
    _states ^= states;
}

// accessors
unsigned States::getFullStates() const {
    return _states;
}
bool States::getState(StateBit state) const {
    return _states&state;
}
bool States::anyStates(unsigned states) const {
    return _states&states;
}
bool States::allStates(unsigned states) const {
    return (_states&states) == states;
}

// specific state combos
bool States::isInteractable() const {
    // returns true when neither Disabled nor Hidden
    return !(_states & (Disabled|Hidden));
}