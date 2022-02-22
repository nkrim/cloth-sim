/* **************************************************************************** *
 * AUTHOR:      Noah Krim
 * ASSIGNMENT:  GUI Cloth Sim
 * CLASS:       CS_08
 * ---------------------------------------------------------------------------- *
 * File: letter.h
 *  Header file for letter class
 * **************************************************************************** */

#ifndef LETTER_H
#define LETTER_H

#include <SFML/Graphics.hpp>
#include <iostream>

class Letter : public sf::Text {
private:
	// Hide certain troublesome functions from public access
	using sf::Text::setString;

public:
	// global constant
	static const unsigned DEFAULT_LETTER_SIZE = 30;
	static const unsigned SPACES_PER_TAB = 4;

	// constructors
	Letter();
	Letter(sf::Uint32 character);
	Letter(sf::Uint32 character, const sf::Font& font, unsigned int letter_size=DEFAULT_LETTER_SIZE);
	Letter(	sf::Uint32 character, const sf::Font& font, unsigned int letter_size, 
			const sf::Color& fill_color);
	Letter(	sf::Uint32 character, const sf::Font& font, unsigned int letter_size, 
		   	const sf::Color& fill_color, const sf::Color& outline_color, float outline_thickness=0);

	// mutators
	bool setChar(sf::Uint32 character);	// (return bool for potential validation purposes)

	// accessors
	sf::Uint32 getChar() const;
	float getWidth() const;
	float getGlyphWidth() const;
	float getAdvanceOffset() const;
	sf::FloatRect getLocalLetterBounds() const;
	sf::FloatRect getGlobalLetterBounds() const;

	// conversions
	operator sf::Uint32() const;
	operator char() const;

	// comparison
	friend bool operator==(const Letter& a, const Letter& b);
	friend bool operator!=(const Letter& a, const Letter& b);
	friend bool operator< (const Letter& a, const Letter& b);
	friend bool operator<=(const Letter& a, const Letter& b);
	friend bool operator> (const Letter& a, const Letter& b);
	friend bool operator>=(const Letter& a, const Letter& b);

	// io
	friend std::ostream& operator<<(std::ostream& os, const Letter& l);
};

#endif