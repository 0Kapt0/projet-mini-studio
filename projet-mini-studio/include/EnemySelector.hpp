#ifndef ENEMY_SELECTOR_HPP
#define ENEMY_SELECTOR_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace sf;

enum class EnemyType {
    None,
    Flying,
    Ranged,
    Basic,
    ChargingBoss,
    FlyingBoss,
    Checkpoint,
    HeartItem,
};

class EnemySelector {
public:
    EnemySelector(const string& texturePath, int iconSize);
    void handleEvent(Event event, RenderWindow& window);
    void draw(RenderWindow& window);
    EnemyType getSelectedEnemy() const;

private:
    Texture m_texture;
    vector<Sprite> m_sprites;
    int m_iconSize;
    int m_selectedIndex;

    EnemyType m_selectedEnemy;

    vector<EnemyType> m_enemyTypes;
};

#endif
