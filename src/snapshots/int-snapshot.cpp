/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: int-snapshot.cpp
 *  Definition file for IntSnapshot
 * **************************************************************************** */

#include "int-snapshot.h"

IntSnapshot::IntSnapshot(Snapshotable& source)
    :Snapshot{source}
    , _before{0}, _after{0}
{ }
IntSnapshot::IntSnapshot(Snapshotable& source, int before, int after)
    :Snapshot{source}
    , _before{before}, _after{after}
{ }

// destructor
IntSnapshot::~IntSnapshot() 
{ }

// inherited from Snapshot
Snapshot* IntSnapshot::clone() const {
    return new IntSnapshot(*this);
}

// accessors
int IntSnapshot::getBefore() const {
    return _before;
}
int IntSnapshot::getAfter() const {
    return _after;
}
