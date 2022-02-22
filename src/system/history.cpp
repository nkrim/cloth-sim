/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: history.cpp
 *  Definition file for History class
 * **************************************************************************** */

#include "history.h"
#include "key-shortcuts.h"

// constructors
History::History() 
    : _stack{}, _stack_top{}
{ }

// destructor
History::~History() {
    clear();
}

// inherited from EventHandler
bool History::addEventHandler(sf::RenderWindow& window, const sf::Event& event) {
    if(event.type == sf::Event::KeyPressed) {
        auto mods = KeyShortcuts::getKeyModifiersPressed();
        bool ctrl_pressed = mods&KeyShortcuts::Ctrl;
        bool shift_pressed = mods&KeyShortcuts::Shift;

        // shift+ctrl+z OR ctrl+y -> redo
        if( (event.key.code == sf::Keyboard::Z && shift_pressed && ctrl_pressed)
                || (event.key.code == sf::Keyboard::Y && ctrl_pressed) ) 
        {
            redo();
        }
        // ctrl+z -> undo
        else if(event.key.code == sf::Keyboard::Z && ctrl_pressed) {
            undo();
        }
        
    }

    // has no snapshot itself so always return false
    return false;
}
void History::update(float t) { }

// history endpoints
unsigned History::size() {
    return _stack.size();
}
void History::pushSnapshot(const Snapshotable& component) {
    // start by clearing redos (pushes top to end)
    clearRedos();

    // grab snapshot from component and push to stack
    _stack.insert_tail(component.getSnapshot().clone());
}
bool History::undo() {
    // if empty stack or top at beginning, no more undos, exit early
    if(_stack.empty() || _stack_top == _stack.begin())
        return false;

    // manually push stack to tail when at end
    if(_stack_top == _stack.end())
        _stack_top = _stack.tail_iter();
    // otherwise decrement towards beginning
    else
        --_stack_top;

    // revert snapshot on source
    (*_stack_top)->getSource()->revertSnapshot(**_stack_top);

    // signal success
    return true;
}
bool History::redo() {
    // if empty stack or top at end, no more redos, exit early
    if(_stack_top == _stack.end())
        return false;

    // apply snapshot on source
    (*_stack_top)->getSource()->applySnapshot(**_stack_top);

    // increment top towards end
    ++_stack_top;

    // signal success
    return true;
}
Snapshot* History::nextUndo() {
    if(_stack.empty() || _stack_top == _stack.begin())
        return nullptr;
    auto top_copy = _stack_top;
    return *(--top_copy);
}
Snapshot* History::nextRedo() {
    if(_stack_top == _stack.end())
        return nullptr;
    return *_stack_top;
}

void History::clear() {
    // delete snapshots
    for(auto it=_stack.begin(); it!=_stack.end(); ++it)
        delete *it;
    // clear linked list
    _stack.clear();
    _stack_top = _stack.end();
}
void History::clearRedos() {
    // if at end, no more redos, exit early
    if(_stack_top == _stack.end())
        return;

    // delete snapshot at and above _stack_top
    for(auto it=_stack_top; it!=_stack.end(); ++it)
        delete *it;

    // erase elements from list
    _stack.remove(_stack_top, _stack.end());
    _stack_top = _stack.end();
}