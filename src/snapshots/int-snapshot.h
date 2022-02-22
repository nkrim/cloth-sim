/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: int-snapshot.h
 *  Header file for IntSnapshot
 * **************************************************************************** */

#ifndef INT_SNAPSHOT_H
#define INT_SNAPSHOT_H

#include <SFML/Graphics.hpp>

#include "../gui/component-interface/snapshot-interface.h"

class IntSnapshot : public Snapshot {
private:
    int _before;
    int _after;

public:
    // constructors
    IntSnapshot(Snapshotable& source);
    IntSnapshot(Snapshotable& source, 
                int before, int after);

    // destructor
    virtual ~IntSnapshot();

    // inherited from Snapshot
    virtual Snapshot* clone() const;

    // accessors
    int getBefore() const;
    int getAfter() const;
};

#endif