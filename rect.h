#include <SFML/Graphics.hpp>

struct Rect {
    float PosX, PosY, Width, Height;
    sf::Vector2f getPosition() { return {PosX, PosY}; }
    void setPosition(sf::Vector2f v) { PosX = v.x; PosY = v.y; }
    void setPosition(float x, float y) { PosX = x; PosY = y; }
    sf::Vector2f getSize() { return {Width, Height}; }
    void setSize(sf::Vector2f v) { setSize(v.x, v.y); }
    void setSize(float w, float h) { Width = w; Height = h; }
    void setRect(float x, float y, float w, float h) { PosX = x; PosY = y; Width = w; Height = h; }
};