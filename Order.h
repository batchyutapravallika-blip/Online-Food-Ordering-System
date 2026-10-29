
#ifndef ORDER_H
#define ORDER_H

#include "FoodItem.h"
#include <string>
#include <vector>
using namespace std;

struct OrderItem {
    int foodId;
    string foodName;
    int price;
    int quantity;
};

class Order {
private:
    int orderId;
    string customerName;
    vector<OrderItem> items;
    string status;

public:
    Order(int id, string customer);

    int getOrderId() const;
    string getCustomerName() const;
    string getStatus() const;
    const vector<OrderItem>& getItems() const;

    void addItem(const FoodItem& food, int quantity);
    void setStatus(string s);

    int calculateBill() const;
    int calculateBill(int discountPercent) const;

    void display() const;
};

#endif
