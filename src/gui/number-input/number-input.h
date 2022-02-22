/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: number-input.h
 *  Header file for NumberInput class
 * **************************************************************************** */

#ifndef NUMBER_INPUT_H
#define NUMBER_INPUT_H

#include <SFML/Graphics.hpp>

#include "../text-input/text-input.h"

class NumberInput : public TextInput {
public:
    // enums
    enum NumberType {
        Int=0,
        Uint,
        Float,
        Hex32
    };

private:
    sf::String _validated_text;
    NumberType _type;
    int _int_value;
    uint32_t _uint_value;
    float _float_value;
    int _min_int_value;
    int _max_int_value;
    float _min_float_value;
    float _max_float_value;
    bool _need_parse;
    bool _focused_last_frame;

public:
    // constructors
    NumberInput();
    NumberInput(NumberType number_type,
            const sf::Font& font, const sf::Vector2u& dimensions, 
            unsigned char_size=Letter::DEFAULT_LETTER_SIZE);
    NumberInput(NumberType number_type, const sf::String& label, 
            const sf::Font& font, const sf::Vector2u& dimensions,
            unsigned char_size=Letter::DEFAULT_LETTER_SIZE);
    NumberInput(const NumberInput& o);

    // virtual destructor
    virtual ~NumberInput();

    // inhereted from GuiComponent (EventHandler)
    virtual bool addEventHandler(sf::RenderWindow& window, const sf::Event& event);
    virtual void update(float t=0);

    // number input accessors
    NumberType getNumberType() const;
    int getIntValue();
    uint32_t getUintValue();
    float getFloatValue();
    int getMinIntValue() const;
    int getMaxIntValue() const;
    float getMinFloatValue() const;
    float getMaxFloatValue() const;

    // number input mutators
    void setNumberType(NumberType type);
    bool setIntValue(int value);
    bool setHexValue(uint32_t value);
    bool setFloatValue(float value);
    bool setMinIntValue(int value);
    bool setMaxIntValue(int value);
    bool setMinFloatValue(float value);
    bool setMaxFloatValue(float value);

    // value change
    void requestParse();
    void resetValue();
    bool incrementValue();
    bool decrementValue();

private:
    // validation
    bool _validate();
};

#endif