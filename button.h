#pragma once
#include "text.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Button : public PlaccedText {
public:
    bool Pushed;
    void (*buttonFunction)(void);
    sf::RectangleShape rect;

    Button(str, void (*)(void));
    void setWord(str);
    void setPosition(float, float);
    void draw(sf::RenderWindow&);
    bool OnTheButton(int, int);
    bool isActivated(sf::Event&);
    void setSize(float, float);
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Button::Button(str word, void (*foo)(void)) : PlaccedText(word) {
    setlocale(LC_ALL, "rus");
    setWord(word);
    this->buttonFunction = foo;
    rect.setFillColor(sf::Color(255, 255, 255, 0));
    rect.setOutlineColor(sf::Color::Black);
    rect.setOutlineThickness(3);
    text.setFillColor(sf::Color::Black);
}

void Button::setWord(str word) {
    text.setString(word);
    text.setPosition(PosX + Width / 2 - text.getGlobalBounds().width / 2,
                     PosY + Height / 2 - text.getGlobalBounds().height / 2);
}

void Button::setPosition(float x, float y) {
    PosX = x; PosY = y;
    rect.setPosition(x, y);
    text.setPosition(x + Width / 2 - text.getGlobalBounds().width / 2,
                     y + Height / 2 - text.getGlobalBounds().height / 2);
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(rect);
    window.draw(text);
}

bool Button::OnTheButton(int x, int y) {
    return PosX <= x && x <= PosX + Width && PosY <= y && y <= PosY + Height;
}

bool Button::isActivated(sf::Event& event) {
    if (Pushed && event.type == sf::Event::MouseButtonReleased) {
        Pushed = false;
        if (OnTheButton(event.mouseButton.x, event.mouseButton.y)) {
            buttonFunction();
            return true;
        }
    }
    if (event.type == sf::Event::MouseButtonPressed &&
        OnTheButton(event.mouseButton.x, event.mouseButton.y) && 
        event.mouseButton.button == sf::Mouse::Button::Left) {
            Pushed = true;
        }
    return false;
}

void Button::setSize(float w, float h) {
    Width = w;
    Height = h;
    rect.setSize(sf::Vector2f(w, h));
    text.setPosition(PosX + Width / 2 - text.getGlobalBounds().width / 2,
                     PosY + Height / 2 - text.getGlobalBounds().height / 2);
}