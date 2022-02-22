/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: letter.cpp
 *  Definition file for letter class
 * **************************************************************************** */

#include "letter.h"

/* ============================================================================ *
 * Constructors
 * ============================================================================ */
Letter::Letter()
	:Letter{'\0'}
{ }

Letter::Letter(sf::Uint32 character) 
	:sf::Text{}
{
	setString(sf::String(character));
}

Letter::Letter(sf::Uint32 ch, const sf::Font& font, unsigned int letter_size)
	:sf::Text{ch, font, letter_size}
{ }

Letter::Letter(	sf::Uint32 ch, const sf::Font& font, unsigned int letter_size, 
				const sf::Color& fill_color)
	:sf::Text{ch, font, letter_size}
{ 
	setFillColor(fill_color);
}

Letter::Letter(	sf::Uint32 ch, const sf::Font& font, unsigned int letter_size, 
	   			const sf::Color& fill_color, const sf::Color& outline_color, float outline_thickness)
	:sf::Text{ch, font, letter_size}
{ 
	setFillColor(fill_color);
	setOutlineColor(outline_color);
	setOutlineThickness(outline_thickness);
}


/* ============================================================================ *
 * Mutators
 * ============================================================================ */
// (return bool for potential validation purposes)
bool Letter::setChar(sf::Uint32 ch) {
	setString(sf::String(ch));
	return true;
}


/* ============================================================================ *
 * Accessors
 * ============================================================================ */
sf::Uint32 Letter::getChar() const {
	return getString()[0];
}

float Letter::getWidth() const {
	return getLocalBounds().width;
}

float Letter::getGlyphWidth() const {
	return getFont()->getGlyph(
			getChar(), getCharacterSize(), 
			getStyle()&sf::Text::Bold, getOutlineThickness()
		).advance;
}

float Letter::getAdvanceOffset() const {
	if(getChar() == '\n')
		return 0;
	if(getChar() == '\t')
		return getFont()->getGlyph(
				' ', getCharacterSize(), 
				getStyle()&sf::Text::Bold, getOutlineThickness()
			).advance * SPACES_PER_TAB;
	return getGlyphWidth() * getLetterSpacing();
}
sf::FloatRect Letter::getLocalLetterBounds() const {
	return sf::FloatRect({0,0}, {getAdvanceOffset(), (float)getCharacterSize()});
}
sf::FloatRect Letter::getGlobalLetterBounds() const {
	return getTransform().transformRect(getLocalLetterBounds());
}


/* ============================================================================ *
 * Conversions
 * ============================================================================ */
Letter::operator sf::Uint32() const {
	return getString()[0];
}

Letter::operator char() const {
	return (char)(getString()[0]);
}


/* ============================================================================ *
 * Comparison
 * ============================================================================ */
bool operator==(const Letter& a, const Letter& b) {
	return a.getChar() == b.getChar();
}
bool operator!=(const Letter& a, const Letter& b) {
	return a.getChar() != b.getChar();
}
bool operator< (const Letter& a, const Letter& b) {
	return a.getChar() <  b.getChar();
}
bool operator<=(const Letter& a, const Letter& b) {
	return a.getChar() <= b.getChar();
}
bool operator> (const Letter& a, const Letter& b) {
	return a.getChar() >  b.getChar();
}
bool operator>=(const Letter& a, const Letter& b) {
	return a.getChar() >= b.getChar();
}


/* ============================================================================ *
 * IO
 * ============================================================================ */
std::ostream& operator<<(std::ostream& os, const Letter& l) {
	return os << l.getChar();
}