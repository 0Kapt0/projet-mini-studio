#include "../include/DropDown.hpp"
#include <iostream>

DropDown::DropDown(const Font& font,
    const vector<string>& items,
    Vector2f position,
    Vector2f size)
    : m_items(items)
    , m_selectedIndex(0)
    , m_isOpen(false)
    , m_position(position)
    , m_size(size)
{
    m_font = font;

    m_mainBox.setPosition(m_position);
    m_mainBox.setSize(m_size);
    m_mainBox.setFillColor(Color(180, 180, 180));
    m_mainBox.setOutlineColor(Color::Black);
    m_mainBox.setOutlineThickness(2.f);

    m_textSelected.setFont(m_font);
    if (!m_items.empty()) {
        m_textSelected.setString(m_items[m_selectedIndex]);
    }
    m_textSelected.setCharacterSize(18);
    m_textSelected.setFillColor(Color::Black);

    m_textSelected.setPosition(m_position.x + 5.f, m_position.y + 3.f);
}

void DropDown::handleEvent(const Event& event, RenderWindow& window)
{
    if (event.type == Event::MouseButtonPressed) {
        if (event.mouseButton.button == Mouse::Left) {
            Vector2i mousePos = Mouse::getPosition(window);

            FloatRect mainBoxBounds(m_mainBox.getPosition().x,
                m_mainBox.getPosition().y,
                m_mainBox.getSize().x,
                m_mainBox.getSize().y);

            if (mainBoxBounds.contains((float)mousePos.x, (float)mousePos.y)) {
                m_isOpen = !m_isOpen;
            }
            else if (m_isOpen) {

                for (int i = 0; i < (int)m_items.size(); i++) {
                    FloatRect itemBounds(
                        m_position.x,
                        m_position.y + m_size.y * (1 + i),
                        m_size.x,
                        m_size.y
                    );

                    if (itemBounds.contains((float)mousePos.x, (float)mousePos.y)) {
                        m_selectedIndex = i;
                        m_textSelected.setString(m_items[m_selectedIndex]);
                        m_isOpen = false;
                        break;
                    }
                }
            }
            else {

            }
        }
    }
}

void DropDown::draw(RenderWindow& window)
{
    window.draw(m_mainBox);

    window.draw(m_textSelected);

    if (m_isOpen) {
        for (int i = 0; i < (int)m_items.size(); i++) {
            RectangleShape boxItem;
            boxItem.setPosition(m_position.x, m_position.y + m_size.y * (1 + i));
            boxItem.setSize(m_size);
            boxItem.setFillColor(Color(200, 200, 200));
            boxItem.setOutlineColor(Color::Black);
            boxItem.setOutlineThickness(1.f);
            window.draw(boxItem);

            Text textItem;
            textItem.setFont(m_font);
            textItem.setString(m_items[i]);
            textItem.setCharacterSize(18);
            textItem.setFillColor(Color::Black);

            textItem.setPosition(m_position.x + 5.f, m_position.y + m_size.y * (1 + i) + 3.f);

            window.draw(textItem);
        }
    }
}

string DropDown::getSelectedItem() const
{
    if (m_selectedIndex >= 0 && m_selectedIndex < (int)m_items.size()) {
        return m_items[m_selectedIndex];
    }
    return "";
}
