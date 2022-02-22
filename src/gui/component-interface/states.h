/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: states.h
 *  Header file for States class
 * **************************************************************************** */

#ifndef STATES_H
#define STATES_H

class States {
private:
    unsigned _states;

public:
    // StateBits enum for masking bits
    enum StateBit : unsigned {
        Disabled    = 1<<0,
        Hidden      = 1<<1,
        Unclickable = 1<<2,
        Selected    = 1<<3,
        Focused     = 1<<4,
        Clicked     = 1<<5,
        Hovered     = 1<<6
    };

    // constructor
    States();
    States(unsigned states);

    // mutators
    void setState(StateBit state, bool value);
    void setStates(unsigned states, bool value);
    void setAllStates(unsigned states);
    void toggleState(StateBit state);
    void toggleStates(unsigned states);

    // accessors
    unsigned getFullStates() const;
    bool getState(StateBit state) const;
    bool anyStates(unsigned states) const;
    bool allStates(unsigned states) const;

    // specific state combos
    bool isInteractable() const;
};

#endif