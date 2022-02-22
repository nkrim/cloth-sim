/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: string-change-snapshot.h
 *  Header file for StringChangeSnapshot
 * **************************************************************************** */

#ifndef STRING_CHANGE_SNAPSHOT_H
#define STRING_CHANGE_SNAPSHOT_H

#include <SFML/Graphics.hpp>

#include "../gui/component-interface/snapshot-interface.h"

class StringChangeSnapshot : public Snapshot {
private:
    sf::String _inserted_data;
    sf::String _erased_data;
    unsigned _start_index;

public:
    // constructors
    StringChangeSnapshot(Snapshotable& source);
    StringChangeSnapshot(Snapshotable& source, 
                        const sf::String& inserted_data, 
                        const sf::String& erased_data,
                        unsigned start_index);

    // destructor
    virtual ~StringChangeSnapshot();

    // inherited from Snapshot
    virtual Snapshot* clone() const;

    // accessors
    const sf::String& getInserted() const;
    const sf::String& getErased() const;
    unsigned getStartIndex() const;
};

#endif