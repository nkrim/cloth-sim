/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: string-change-snapshot.cpp
 *  Definition file for StringChangeSnapshot
 * **************************************************************************** */

#include "string-change-snapshot.h"

// constructors
StringChangeSnapshot::StringChangeSnapshot(Snapshotable& source)
    :Snapshot{source}
    , _inserted_data()
    , _erased_data()
    , _start_index(0)
{ }
StringChangeSnapshot::StringChangeSnapshot(Snapshotable& source, 
        const sf::String& inserted_data, const sf::String& erased_data,
        unsigned start_index)
    :Snapshot{source}
    , _inserted_data(inserted_data)
    , _erased_data(erased_data)
    , _start_index(start_index)
{ }

// destructor
StringChangeSnapshot::~StringChangeSnapshot() 
{ }

// inherited from Snapshot
Snapshot* StringChangeSnapshot::clone() const {
    return new StringChangeSnapshot(*this);
}

// accessors
const sf::String& StringChangeSnapshot::getInserted() const { return _inserted_data; }
const sf::String& StringChangeSnapshot::getErased() const { return _erased_data; }
unsigned StringChangeSnapshot::getStartIndex() const { return _start_index; }