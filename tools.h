#include "SFML-2.5.1/include/SFML/Graphics.hpp"
#include <bits/stdc++.h>
#include "scale.h"
#include "Matrix.h"
#include "button.h"
using namespace std;
using str = std::string;
using Point = sf::Vector2f;
using vp = std::vector<Point>;

////////////////////////////////////////////////////////////

int scw = 1200, sch = 800;
float pointRad = 10.f;
float MaxX = 10, MinX = -10, MaxY = 10, MinY = -10;
float zoom = std::pow(1.1, 50);
bool showPoints = true, mapMoved = false;
Point* ActivePoint = nullptr, *ActiveHoldingPoint = nullptr, MainPoint;
sf::Vector2f Xrange = {-10, 10}, Yrange = {-10, 10};

float stepBy = 1;
float stepsBy[3] = {0.5, 0.4, 0.5};
int IndexStepBy = 0;

stringstream stringStream;

sf::RenderWindow window(sf::VideoMode(scw, sch), "Nasedkin G. K.");
sf::Event event;
sf::Vector2f CameraPos(scw / 2, sch / 2 - 75);
sf::Vector2i MouseBuffer;
sf::Mouse Mouse;
sf::Font font; sf::Text AxisText;
sf::CircleShape PointShape(pointRad), MainPointShape(pointRad);
vp points(0);
float InterfaceWidth = 150;
sf::RectangleShape InterfacePlato(sf::Vector2f(scw, InterfaceWidth));

sf::VertexArray VertexOfAxisLine(sf::Lines, 2);

////////////////////////////////////////////////////////////
// Funcs
////////////////////////////////////////////////////////////

void DrawGrid();
void DrawInterface();
void DrawAll();
void moveWindow();
void movePoint();
void MakePolygon();
void Update();
void EventHandling();
void init();
sf::Vector2f ConvertXY(sf::Vector2f v) { return (v - CameraPos) / zoom; }
float ConvertX(float x) { return (x - CameraPos.x) / zoom; }
float ConvertY(float y) { return (y - CameraPos.y) / zoom; }
void addPoint(float x, float y) { points.emplace_back(x, y); }
void addPoint(sf::Vector2f v) { addPoint(v.x, v.y); }
Point* get_active(sf::Vector2f);

float XAxisPlace(sf::Text text) {
    if (ConvertY(0) < 0 && ConvertY(sch - InterfaceWidth - text.getGlobalBounds().height - 15) > 0)
        return 6.f;
    return ((ConvertY(0) >= 0) ? 0 : sch - InterfaceWidth - text.getGlobalBounds().height - 10) - CameraPos.y;
}
// ------------------------------------------------------------
float YAxisPlace(sf::Text text) {
    if (ConvertX(-7) < 0 && ConvertX(scw - text.getGlobalBounds().width - 7) > 0)
        return 6.f;
    return ((ConvertX(-7) >= 0) ? 0 : scw - text.getGlobalBounds().width - 2) - CameraPos.x;
}
// ------------------------------------------------------------
void SetAxisTextByFloatX(float x) {
    stringStream.str(""); stringStream << x;
    AxisText.setString(stringStream.str());
    AxisText.setPosition(sf::Vector2f(x * zoom, XAxisPlace(AxisText)) + CameraPos);
}
// ------------------------------------------------------------
void SetAxisTextByFloatY(float y) {
    stringStream.str(""); stringStream << y;
    AxisText.setString(stringStream.str());
    AxisText.setPosition(sf::Vector2f(YAxisPlace(AxisText), -y * zoom) + CameraPos);
}
// ------------------------------------------------------------
void SetVertexOfAxisLineX(float x) {
    VertexOfAxisLine[0] = sf::Vertex(sf::Vector2f(CameraPos.x + x * zoom, 0), sf::Color::Black);
    VertexOfAxisLine[1] = sf::Vertex(sf::Vector2f(CameraPos.x + x * zoom, sch), sf::Color::Black);
}
// ------------------------------------------------------------
void SetVertexOfAxisLineY(float y) {
    VertexOfAxisLine[0] = sf::Vertex(sf::Vector2f(0, CameraPos.y - y * zoom), sf::Color::Black);
    VertexOfAxisLine[1] = sf::Vertex(sf::Vector2f(scw, CameraPos.y - y * zoom), sf::Color::Black);
}

////////////////////////////////////////////////////////////
// Buttons
////////////////////////////////////////////////////////////

Button AddPointButton("Add point", [](){
    addPoint(0, 0);
    ActivePoint = &points[points.size() - 1];
});
Button DelPointButton("Delete point", [](){
    if (ActivePoint != nullptr && ActivePoint != &MainPoint)
        for (auto i = points.begin(); i != points.end(); i++)
            if (*i == *ActivePoint) {
                points.erase(i);
                ActivePoint = nullptr;
                break;
            }
});
PlaccedText XOfCurPointText("x = ");
PlaccedText YOfCurPointText("y = ");

////////////////////////////////////////////////////////////

void DrawGrid() {
    AxisText.setPosition(0, 0);
    stringStream.str(""); stringStream <<  "x: " << (MouseBuffer.x - CameraPos.x) / zoom <<
                                                "\ny: " << (-MouseBuffer.y + CameraPos.y) / zoom;
    AxisText.setString(stringStream.str());
    window.draw(AxisText);
    // axis
    for (float x = min(-1, int(ConvertX(scw) / stepBy)) * stepBy; x >= ConvertX(0); x -= stepBy) { // x < 0
        SetAxisTextByFloatX(x); SetVertexOfAxisLineX(x);
        window.draw(AxisText); window.draw(VertexOfAxisLine);
    }
    for (float x = max(0, int(ConvertX(0) / stepBy)) * stepBy; x <= ConvertX(scw); x += stepBy) { // x >= 0
        SetAxisTextByFloatX(x); SetVertexOfAxisLineX(x);
        window.draw(AxisText); window.draw(VertexOfAxisLine);
    }
    for (float y = min(-1, int(-ConvertY(0) / stepBy)) * stepBy; y >= -ConvertY(sch - InterfaceWidth); y -= stepBy) { // y < 0
        SetAxisTextByFloatY(y); SetVertexOfAxisLineY(y);
        window.draw(AxisText); window.draw(VertexOfAxisLine);
    }
    for (float y = max(1, int(-ConvertY(sch - InterfaceWidth) / stepBy)) * stepBy; y <= -ConvertY(0); y += stepBy) { // y > 0
        SetAxisTextByFloatY(y); SetVertexOfAxisLineY(y);
        window.draw(AxisText); window.draw(VertexOfAxisLine);
    }
    SetVertexOfAxisLineY(0);
    window.draw(VertexOfAxisLine);
}

// ------------------------------------------------------------

void DrawInterface() {
    window.draw(InterfacePlato);

    stringStream.str(""); stringStream << "x = "; if (ActivePoint != nullptr) stringStream << ActivePoint->x;
    XOfCurPointText.setText(stringStream.str());

    stringStream.str(""); stringStream << "y = "; if (ActivePoint != nullptr) stringStream << -ActivePoint->y;
    YOfCurPointText.setText(stringStream.str());

    XOfCurPointText.draw(window);
    YOfCurPointText.draw(window);

    AddPointButton.draw(window);
    DelPointButton.draw(window);
}

// ------------------------------------------------------------

void DrawAll() {
    window.clear(sf::Color::White);
    DrawGrid();
    // points
    if (points.size() > 2) {
        sf::VertexArray line(sf::TriangleFan, points.size() + 1);
        for (int i = 0; i < points.size(); i++)
            line[i] = sf::Vertex(points[i] * zoom + CameraPos, sf::Color(0, 0, 0, 120));
        line[points.size()] = sf::Vertex(points[0] * zoom + CameraPos, sf::Color(0, 0, 0, 120));
        window.draw(line);
    }
    if (showPoints)
        for (int i = 0; i < points.size(); i++) {
            PointShape.setPosition(sf::Vector2f{points[i].x * zoom - pointRad,
                points[i].y * zoom - pointRad} + CameraPos);
            window.draw(PointShape);
        }
    MainPointShape.setPosition(sf::Vector2f{MainPoint.x * zoom - pointRad,
                MainPoint.y * zoom - pointRad} + CameraPos);
    window.draw(MainPointShape);
    DrawInterface();

    window.display();
}

// ------------------------------------------------------------

void moveWindow() {
    if (window.hasFocus() && mapMoved)
        CameraPos += sf::Vector2f(Mouse.getPosition(window) - MouseBuffer);
    MouseBuffer = Mouse.getPosition(window);
}

// ------------------------------------------------------------

void movePoint() {
    if (ActiveHoldingPoint != nullptr)
        *ActiveHoldingPoint = ConvertXY(sf::Vector2f(Mouse.getPosition(window)));
}

// ------------------------------------------------------------

Point CenterOfPolygon;
void MakePolygon() {
    bool DoneSuccessfully = true;
    CenterOfPolygon = {0, 0};
    for (Point& cur: points)
        CenterOfPolygon += cur;
    CenterOfPolygon /= float(points.size());
    sort(points.begin(), points.end(), [](Point& left, Point& right){
        return atan2(left.x - CenterOfPolygon.x, left.y - CenterOfPolygon.y) < atan2(right.x - CenterOfPolygon.x, right.y - CenterOfPolygon.y);
    });
}

// ------------------------------------------------------------

void Update() {
    moveWindow();
    movePoint();
    Point WasHolding; if (ActiveHoldingPoint != nullptr) WasHolding = *ActiveHoldingPoint;
    MakePolygon();
    if (ActiveHoldingPoint != nullptr)
        for (Point& cur: points)
            if (cur == WasHolding)
                ActiveHoldingPoint = &cur;
}

// ------------------------------------------------------------

void EventHandling() {
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) window.close();
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) window.close();
            if (event.key.code == sf::Keyboard::H) showPoints = !showPoints;
        } else if (event.type == sf::Event::MouseWheelScrolled) {
            if (MouseBuffer.y < 650) {
                float coef = std::pow(1.1, event.mouseWheelScroll.delta);
                zoom *= coef;
                CameraPos = (sf::Vector2f)MouseBuffer - ((sf::Vector2f)MouseBuffer - CameraPos) * coef;

                if (stepBy * zoom > 190) {
                    stepBy = max(0.001f, stepBy * stepsBy[IndexStepBy]);
                    IndexStepBy = (IndexStepBy + 1) % 3;
                } else if (stepBy * zoom < 80) {
                    IndexStepBy = (IndexStepBy + 2) % 3;
                    stepBy /= stepsBy[IndexStepBy];
                }
            }
        } else if (event.type == sf::Event::MouseButtonPressed) {
            if (MouseBuffer.y < 650) {
                ActiveHoldingPoint = get_active(ConvertXY(sf::Vector2f(MouseBuffer)));
                ActivePoint = (ActiveHoldingPoint == nullptr) ? ActivePoint : ActiveHoldingPoint;
                if (ActiveHoldingPoint == nullptr)
                    mapMoved = true;
            }
        } else if (event.type == sf::Event::MouseButtonReleased) {
            mapMoved = false;
            ActiveHoldingPoint = nullptr;
        } else if (event.type == sf::Event::KeyReleased) {
            
        }
        AddPointButton.isActivated(event);
        DelPointButton.isActivated(event);
    }
}

// ------------------------------------------------------------

void init() {
    font.loadFromFile("arial.ttf");

    AxisText.setFont(font);
    AxisText.setCharacterSize(14);
    AxisText.setFillColor(sf::Color::Black);

    PointShape.setFillColor(sf::Color(0, 255, 0, 180));
    PointShape.setRadius(pointRad);

    MainPoint.x = 0; MainPoint.y = 0;
    MainPointShape.setFillColor(sf::Color(255, 0, 0, 180));
    MainPointShape.setRadius(pointRad);

    InterfacePlato.setOutlineColor(sf::Color::Black);
    InterfacePlato.setOutlineThickness(5);
    InterfacePlato.setFillColor(sf::Color::White);
    InterfacePlato.setPosition(0, 650);

    AddPointButton.setSize(200, 40);
    AddPointButton.setPosition(750, 680);
    
    DelPointButton.setSize(200, 40);
    DelPointButton.setPosition(750, 740);
    
    XOfCurPointText.setSize(200, 40);
    XOfCurPointText.setPosition(975, 680);
    XOfCurPointText.text.setFillColor(sf::Color::Black);
    
    YOfCurPointText.setSize(200, 40);
    YOfCurPointText.setPosition(975, 740);
    YOfCurPointText.text.setFillColor(sf::Color::Black);
}

// ------------------------------------------------------------

Point* get_active(sf::Vector2f v) {
    for (Point& cur: points)
        if (hypot(v.x - cur.x, v.y - cur.y) <= pointRad / zoom)
            return &cur;
    if (hypot(v.x - MainPoint.x, v.y - MainPoint.y) <= pointRad / zoom)
        return &MainPoint;
    return nullptr;
}