/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: snapshot-interface.h
 *  Header file for abstract Snapshot and abstract Snapshotable classes
 * **************************************************************************** */

#ifndef SNAPSHOT_INTERFACE_H
#define SNAPSHOT_INTERFACE_H

class Snapshot {
private:
    class Snapshotable* _snapshot_source;

public:
    // constructor
    Snapshot(Snapshotable& source)
        : _snapshot_source(&source)
    { }

    // destructor
    virtual ~Snapshot() { }

    // clone
    virtual Snapshot* clone() const = 0;

    // accessors
    class Snapshotable* getSource() { return _snapshot_source; } 
};

class Snapshotable {
public:
    virtual const Snapshot& getSnapshot() const = 0;
    virtual void applySnapshot(Snapshot& snapshot) = 0;
    virtual void revertSnapshot(Snapshot& snapshot) = 0;
};

class InvalidSnapshotException : public std::exception {
public:
    // constructor
    InvalidSnapshotException()
    { }

    virtual const char* what() const noexcept {
        return "Snapshot class has no defined action for this Snapshottable class.";
    }
};

#endif