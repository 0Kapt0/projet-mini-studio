#ifndef HEART_ITEM_CPP
#define HEART_ITEM_CPP

#include "Entity.hpp"

class Item : public Entity {
public:

	Item(Texture& texture);
};

class HeartItem : public Item {
public:
	HeartItem(Texture& texture);
};

#endif
