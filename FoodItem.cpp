
#include "FoodItem.h"

FoodItem::FoodItem() {
    id = 0;
    name = "";
    price = 0;
}

FoodItem::FoodItem(int i, string n, int p) {
    id = i;
    name = n;
    price = p;
}

int FoodItem::getId() const {
    return id;
}

string FoodItem::getName() const {
    return name;
}

int FoodItem::getPrice() const {
    return price;
}

void FoodItem::setPrice(int p) {
    if (p >= 0)
        price = p;
}

ostream& operator<<(ostream& out, const FoodItem& f) {
    out << f.id << "  " << f.name << "  Rs." << f.price;
    return out;
}
