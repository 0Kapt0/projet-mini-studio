#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

using namespace sf;
using namespace std;

class DropDown {
public:
    // Constructeur
    DropDown(const Font& font,
        const vector<string>& items,
        Vector2f position = { 50.f, 50.f },
        Vector2f size = { 200.f, 30.f });

    // Gère les clics souris, etc.
    void handleEvent(const Event& event, RenderWindow& window);

    // Dessine le dropdown (replié ou déplié)
    void draw(RenderWindow& window);

    // Retourne l'élément sélectionné (ex: "Lobby.txt")
    string getSelectedItem() const;

private:
    vector<string> m_items;   // la liste des options (noms de fichiers .txt)
    int m_selectedIndex;                // l’index actuel
    bool m_isOpen;                      // vrai si la liste est dépliée
    RectangleShape m_mainBox;       // le rectangle principal
    Font m_font;                    // copie ou référence vers la fonte
    Text m_textSelected;            // le texte de l’élément sélectionné

    // Style / dimensions
    Vector2f m_position;
    Vector2f m_size;
};
