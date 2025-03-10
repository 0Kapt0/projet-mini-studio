//#include "../include/RangedEnemy.hpp"
//
//RangedEnemy::RangedEnemy() : Enemy() {
//    attackCooldown = 2.0f; // Tire toutes les 2 secondes
//    attackTimer = 0;
//}
//
//RangedEnemy::RangedEnemy(const sf::Vector2f& size, const sf::Color& color)
//    : Enemy(size, color) {
//    attackCooldown = 2.0f;
//    attackTimer = 0;
//}
//
//void RangedEnemy::update(float dt) {
//    // Mise à jour du mouvement
//    Enemy::update(dt);
//
//    // Mise à jour du timer de tir
//    attackTimer += dt;
//    if (attackTimer >= attackCooldown) {
//        shoot();
//        attackTimer = 0; // Reset du timer
//    }
//
//    // Mise à jour de la position des projectiles
//    for (auto& projectile : projectiles) {
//        projectile.move(0, 300 * dt); // Déplacement vers le bas
//    }
//}
//
//void RangedEnemy::shoot() {
//    sf::RectangleShape projectile(sf::Vector2f(10, 20));
//    projectile.setFillColor(sf::Color::Red);
//    projectile.setPosition(getSprite().getPosition().x, getSprite().getPosition().y + 20);
//
//    projectiles.push_back(projectile);
//}
//
//void RangedEnemy::drawProjectiles(sf::RenderWindow& window) {
//    for (const auto& projectile : projectiles) {
//        window.draw(projectile);
//    }
//}
