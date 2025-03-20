#include "../include/HeartItem.hpp"

Item::Item(Texture& texture) : Entity(texture) {

}

HeartItem::HeartItem(Texture& texture) : Item(texture){
	type = "HeartItem";
	getSprite().setScale(0.05, 0.05f);
}