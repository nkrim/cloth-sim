/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: key-shortcuts.h
 *  Header file for KeyShortcuts class
 * **************************************************************************** */

#ifndef KEY_SHORTCUTS_H
#define KEY_SHORTCUTS_H

#include <SFML/Graphics.hpp>

class KeyShortcuts {
public:
    // typedef for modifier bitmap
    typedef unsigned char modifier_map;

    // modifier character enum
    enum Modifier : modifier_map {
        Shift   = 1<<0,
        Ctrl    = 1<<1,
        Alt     = 1<<2
    };

    // static shortcut functions
    static bool isKeyModifierPressed(Modifier mod) {
        switch(mod) {
            case Shift:
                return sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)
                    || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
            case Ctrl:
                return sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
                    || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)
                    || sf::Keyboard::isKeyPressed(sf::Keyboard::LSystem)
                    || sf::Keyboard::isKeyPressed(sf::Keyboard::RSystem);
            case Alt:
                return sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)
                    || sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt);
            default:
                return false;
        }
    }

    static modifier_map getKeyModifiersPressed() {
        unsigned char out = 0;
        // shift
        if( sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)
         || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)
        )
            out |= Shift;
        // ctrl
        if( sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)
         || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)
         || sf::Keyboard::isKeyPressed(sf::Keyboard::LSystem)
         || sf::Keyboard::isKeyPressed(sf::Keyboard::RSystem)
        )
            out |= Ctrl;
        // alt
        if( sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)
         || sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt)
        )
            out |= Ctrl;
        // return
        return out;
    }

    static bool allKeyModifiersPressed(modifier_map mods) {
        return (getKeyModifiersPressed()&mods) == mods;
    }
};

#endif