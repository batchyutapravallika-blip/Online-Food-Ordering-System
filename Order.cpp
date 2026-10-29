
#include "Order.h"
#include <iostream>

Order::Order(int id, string customer)
    : orderId(id), customerName(customer), status("Placed") {
}

int Order::getOrderId() const {
    return orderId;
}

string Order::getCustomerName() const {
    return customerName;
}

string Order::getStatus() const {
    return status;
}

const vector<OrderItem>& Order::getItems() const {
    return items;
}

void Order::addItem(const FoodItem& food, int quantity) {
    if (quantity <= 0)
        return;

    for (auto& item : items) {
        if (item.foodId == food.getId()) {
            item.quantity += quantity;
            return;
        }
    }

    OrderItem item;
    item.foodId = food.getId();
    item.foodName = food.getName();
    item.price = food.getPrice();
    item.quantity = quantity;

    items.push_back(item);
}

void Order::setStatus(string s) {
    status = s;
}

int Order::calculateBill() const {
    int total = 0;

    for (const auto& item : items)
        total += item.price * item.quantity;

    return total;
}

int Order::calculateBill(int discountPercent) const {
    int total = calculateBill();

    if (discountPercent < 0 || discountPercent > 100)
        return total;

    return total - total * discountPercent / 100;
}

void Order::display() const {
    cout << "\nOrder ID: " << orderId
         << "\nCustomer: " << customerName << '\n';

    for (const auto& item : items) {
        cout << item.foodName
             << " x " << item.quantity
             << " = Rs." << item.price * item.quantity << '\n';
    }

    cout << "Total: Rs." << calculateBill()
         << "\nStatus: " << status << '\n';
}
