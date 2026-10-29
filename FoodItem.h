
#ifndef FOODITEM_H
#define FOODITEM_H

#include <iostream>
#include <string>
using namespace std;

class FoodItem {
private:
    int id;
    string name;
    int price;

public:
    FoodItem();
    FoodItem(int i, string n, int p);

    int getId() const;
    string getName() const;
    int getPrice() const;

    void setPrice(int p);

    friend ostream& operator<<(ostream& out, const FoodItem& f);
};

#endif
