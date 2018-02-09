#include "UI.hpp"

using namespace UI;

Rectangle::Rectangle(SimpleGraphics &graphics, Point p1, Point p2):
    Drawable(graphics),
    m_p1(p1),
    m_p2(p2)
{}

void Rectangle::draw() {
    m_graphics.draw_rect()
    m_p1.first
    m_p1.second
}


Circle::Circle(SimpleGraphics &graphics, Point center, unsigned radius):
    Drawable(graphics),
    m_center(center),
    m_radius(radius)
{}


Button::Button(SimpleGraphics &graphics, TouchControl &touch,
        Point p1, Point p2, std::string text):
    Rectangle(graphics, p1, p2),
    Touchable(touch),
    m_text(text)
{}


DropdownMenu::DropdownMenu(SimpleGraphics &graphics, TouchControl &touch,
        Expand direction, Point p1, Point p2, std::string text):
    Drawable(graphics),
    Touchable(touch),
    m_expandDir(direction),
    m_expander(graphics, touch, p1, p2, text),
    m_buttons()
{}
