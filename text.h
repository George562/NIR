#pragma once
#include "rect.h"

using str = std::string;
////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class PlaccedText : public Rect {
public:
    sf::Font font;
    sf::Text text;

    PlaccedText();
    PlaccedText(str);
    void setPosition(float, float);
    virtual void draw(sf::RenderWindow& window);
    void setText(str s);
    void setCharacterSize(int x);    
    void setWord(str);
    str getWord();
    void setSize(sf::Vector2f v) { setSize(v.x, v.y); }
    void setSize(float, float);
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

PlaccedText::PlaccedText() {
    font.loadFromFile("arial.ttf");
    text.setFont(font);
    text.setCharacterSize(24);
}

PlaccedText::PlaccedText(str word) : PlaccedText() {
    text.setString(word);
}

void PlaccedText::setPosition(float x, float y) {
    PosX = x; PosY = y;
    text.setPosition(x, y);
}

void PlaccedText::setText(str s) {
    text.setString(s);
    Width = text.getGlobalBounds().width;
    Height = text.getGlobalBounds().height;
}

void PlaccedText::setCharacterSize(int x) {
    text.setCharacterSize(x);
    Width = text.getGlobalBounds().width;
    Height = text.getGlobalBounds().height;
}

void PlaccedText::setWord(str word) {
    text.setString(word);
}
str PlaccedText::getWord() {
    return text.getString();
}

void PlaccedText::draw(sf::RenderWindow& window) {
    text.setPosition(PosX, PosY);
    window.draw(text);
}

void PlaccedText::setSize(float w, float h) {
    Width = w;
    Height = h;
}