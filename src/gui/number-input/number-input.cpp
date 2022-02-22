/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: number-input.cpp
 *  Definition file for NumberInput class
 * **************************************************************************** */

#include "number-input.h"

#include <string>
#include <regex>
#include <limits>
#include <sstream>
#include <iomanip>

// constructors
NumberInput::NumberInput()
    :TextInput()
    , _validated_text()
    , _type(Int)
    , _int_value(0)
    , _uint_value(0)
    , _float_value(0.0f)
    , _min_int_value(std::numeric_limits<int>::lowest())
    , _max_int_value(std::numeric_limits<int>::max())
    , _min_float_value(std::numeric_limits<float>::lowest())
    , _max_float_value(std::numeric_limits<float>::max())
    , _need_parse(true)
    , _focused_last_frame(false)
{ 
    resetValue();
}
NumberInput::NumberInput(NumberType number_type,
        const sf::Font& font, const sf::Vector2u& dimensions, 
        unsigned char_size)
    :TextInput(font, dimensions, char_size)
    , _validated_text()
    , _type(number_type)
    , _int_value(0)
    , _uint_value(0)
    , _float_value(0.0f)
    , _min_int_value(std::numeric_limits<int>::lowest())
    , _max_int_value(std::numeric_limits<int>::max())
    , _min_float_value(std::numeric_limits<float>::lowest())
    , _max_float_value(std::numeric_limits<float>::max())
    , _need_parse(true)
    , _focused_last_frame(false)
{ 
    resetValue();
}
NumberInput::NumberInput(NumberType number_type, const sf::String& label,
        const sf::Font& font, const sf::Vector2u& dimensions,
        unsigned char_size)
    :TextInput(label, sf::String(), font, dimensions, char_size)
    , _validated_text()
    , _type(number_type)
    , _int_value(0)
    , _uint_value(0)
    , _float_value(0.0f)
    , _min_int_value(std::numeric_limits<int>::lowest())
    , _max_int_value(std::numeric_limits<int>::max())
    , _min_float_value(std::numeric_limits<float>::lowest())
    , _max_float_value(std::numeric_limits<float>::max())
    , _need_parse(true)
    , _focused_last_frame(false)
{ 
    if(!_validate()) {
        resetValue();
    }
}
NumberInput::NumberInput(const NumberInput& o) 
    :TextInput(o)
    , _validated_text(o._validated_text)
    , _type(o._type)
    , _int_value(0)
    , _uint_value(0)
    , _float_value(0.0f)
    , _min_int_value(o._min_int_value)
    , _max_int_value(o._max_int_value)
    , _min_float_value(o._min_float_value)
    , _max_float_value(o._max_float_value)
    , _need_parse(true)
    , _focused_last_frame(false)
{
    if(!_validate()) {
        resetValue();
    }
}

// virtual destructor
NumberInput::~NumberInput()
{ }

// inhereted from GuiComponent (EventHandler)
bool NumberInput::addEventHandler(sf::RenderWindow& window, const sf::Event& event) {
    if(!getState(States::Focused))
        return false;

    if(TextInput::addEventHandler(window, event)) {
        return true;
    }
    else if(event.type == sf::Event::KeyPressed) {
        if(event.key.code == sf::Keyboard::Enter) {
            _validate();
        }
        else if(event.key.code == sf::Keyboard::Up) {
            sf::String old_text = getText().toString();
            if(incrementValue()) {
                _snapshot = StringChangeSnapshot(*this,
                            getText().toString(),
                            old_text, 0);
                return true;
            }
        }
        else if(event.key.code == sf::Keyboard::Down) {
            sf::String old_text = getText().toString();
            if(decrementValue()) {
                _snapshot = StringChangeSnapshot(*this,
                            getText().toString(),
                            old_text, 0);
                return true;
            }
        }
    }

    return false;
}
void NumberInput::update(float t) {
    TextInput::update(t);
    if(getState(States::Focused)) {
        _focused_last_frame = true;
    }
    else if(_focused_last_frame) {
        _focused_last_frame = false;
        _validate();
    }
}

// number input accessors
NumberInput::NumberType NumberInput::getNumberType() const {
    return _type;
}
int NumberInput::getIntValue() {
    if(_need_parse) {
        _need_parse = false;
        _int_value = strtol(_validated_text.toAnsiString().c_str(), nullptr, 10);
    }
    return _int_value;   
}
uint32_t NumberInput::getUintValue() {
    if(_need_parse) {
        _need_parse = false;
        if(_type == Hex32)
            _uint_value = strtoul(_validated_text.toAnsiString().c_str()+1, nullptr, 16);
        else
            _uint_value = strtoul(_validated_text.toAnsiString().c_str(), nullptr, 10);
    }
    return _uint_value;
}
float NumberInput::getFloatValue() {
    if(_need_parse) {
        _need_parse = false;
        _float_value = strtof(_validated_text.toAnsiString().c_str(), nullptr);
    }
    return _float_value;
}
int NumberInput::getMinIntValue() const {
    return _min_int_value;
}
int NumberInput::getMaxIntValue() const {
    return _max_int_value;
}
float NumberInput::getMinFloatValue() const {
    return _min_float_value;
}
float NumberInput::getMaxFloatValue() const {
    return _max_float_value;
}

// number input mutators
void NumberInput::setNumberType(NumberType type) {
    if(_type == type)
        return;

    _type = type;
    if(_validate()) {
        _need_parse = true;
    }
    else {
        resetValue();
    }
}
bool NumberInput::setIntValue(int value) {
    setTextString(sf::String(std::to_string(value)));
    return _validate();
}
bool NumberInput::setHexValue(uint32_t value) {
    std::stringstream sstream;
    sstream << std::hex << value;
    setTextString(sf::String(sstream.str()));
    return _validate();
}
bool NumberInput::setFloatValue(float value) {
    setTextString(sf::String(std::to_string(value)));
    return _validate();
}
bool NumberInput::setMinIntValue(int value) {
    _min_int_value = value;
    return _validate();
}
bool NumberInput::setMaxIntValue(int value) {
    _max_int_value = value;
    return _validate();
}
bool NumberInput::setMinFloatValue(float value) {
    _min_float_value = value;
    return _validate();
}
bool NumberInput::setMaxFloatValue(float value) {
    _max_float_value = value;
    return _validate();
}

// value change
void NumberInput::requestParse() {
    _need_parse = true;
}
void NumberInput::resetValue() {
    switch(_type) {
        case Int:   setTextString(sf::String("0")); 
                    break;
        case Uint:  setTextString(sf::String("0")); 
                    break;
        case Float: setTextString(sf::String("0.0")); 
                    break;
        case Hex32: setTextString(sf::String("#00000000")); 
                    break;
    }
    _validated_text = getText().toString();
    requestParse();
}
bool NumberInput::incrementValue() {
    // pre-validate to include any valid changes
    _validate();
    switch(_type) {
        case Int:   return setIntValue(getIntValue()+1); 
        case Uint:  return setIntValue(getUintValue()+1); 
        case Float: return setFloatValue(getFloatValue()+1.0f); 
        //case Hex32: 
        default:    return false;
    }
}
bool NumberInput::decrementValue() {
    // pre-validate to include any valid changes
    _validate();
    switch(_type) {
        case Int:   return setIntValue(getIntValue()-1); 
        case Uint:  return setIntValue(getUintValue()-1); 
        case Float: return setFloatValue(getFloatValue()-1.0f); 
        //case Hex32:
        default:    return false;
    }
}

// validation
bool NumberInput::_validate() {
    const std::regex space_regex("\\s+");
    const std::regex trail_zeroes_regex("0+$");
    const std::regex int_regex("^[-+]?\\d+$");
    const std::regex float_regex("^[-+]?\\d+(\\.\\d*)?$");
    const std::regex hex32_regex("^#?[\\da-fA-F]{1,8}$");
    std::smatch m;

    // clear whitespace
    setTextString(sf::String(std::regex_replace(
        getText().toString().toAnsiString(),
        space_regex, std::string())));

    // validate
    bool valid = false;
    std::string to_validate = getText().toString().toAnsiString();
    switch(_type) {
        case Uint:  if(to_validate[0] == '-')
                        break;
                    // continue onto int validation
        case Int:   if(regex_match(to_validate, int_regex)) {
                        if(to_validate[0] == '+') {
                            to_validate.erase(0, 1);
                        }
                        int value = strtol(to_validate.c_str(), nullptr, 10);
                        if(value >= _min_int_value && value <= _max_int_value) {
                            valid = true;
                            setTextString(sf::String(to_validate));
                        }
                    }
                    break;
        case Float: if(regex_match(to_validate, m, float_regex)) {
                        if(to_validate[0] == '+')
                            to_validate.erase(0, 1);
                        if(m[1].length() == 0)
                            to_validate.push_back('.');
                        else
                            to_validate = std::regex_replace(to_validate, trail_zeroes_regex, "");
                        if(to_validate.back() == '.')
                            to_validate.push_back('0');
                        float value = strtof(to_validate.c_str(), nullptr);
                        if(value >= _min_float_value && value <= _max_float_value) {
                            valid = true;
                            setTextString(sf::String(to_validate));
                        }
                    }
                    break;
        case Hex32: 
                    if(regex_match(to_validate, hex32_regex)) {
                        valid = true;
                        if(to_validate[0] != '#')
                            to_validate.insert(0, 1, '#');
                        if(to_validate.size() < 9)
                            to_validate.insert(1, 9-to_validate.size(), '0');
                        setTextString(sf::String(to_validate));
                    }
                    break;
    }

    // finalize
    if(valid) {
        _validated_text = getText().toString();
        requestParse();
        return true;
    }
    else {
        setTextString(_validated_text);
        return false;
    }
}