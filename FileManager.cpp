
#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

vector<FoodItem> FileManager::loadMenu() {
    vector<FoodItem> menu;
    ifstream file("data/menu.txt");

    if (!file) {
        cout << "Could not open data/menu.txt\n";
        return menu;
    }

    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, price;

        if (getline(ss, id, '|') &&
            getline(ss, name, '|') &&
            getline(ss, price, '|')) {
            try {
                int i = stoi(id);
                int p = stoi(price);

                if (i > 0 && p >= 0 &&
                    name.find('|') == string::npos) {
                    menu.push_back(FoodItem(i, name, p));
                }
            } catch (...) {
                cout << "Skipping invalid menu record.\n";
            }
        }
    }

    return menu;
}

bool FileManager::saveMenu(const vector<FoodItem>& menu) {
    for (const auto& f : menu) {
        if (f.getId() <= 0 || f.getPrice() < 0 ||
            f.getName().empty() ||
            f.getName().find('|') != string::npos ||
            f.getName().find('\n') != string::npos ||
            f.getName().find('\r') != string::npos) {
            cout << "Invalid menu data. Nothing was saved.\n";
            return false;
        }
    }

    ofstream file("data/menu.txt");

    if (!file) {
        cout << "Could not open menu.txt for writing.\n";
        return false;
    }

    for (const auto& f : menu) {
        file << f.getId() << '|'
             << f.getName() << '|'
             << f.getPrice() << '\n';
    }

    file.close();

    if (!file) {
        cout << "Error saving menu.\n";
        return false;
    }

    return true;
}

vector<string> FileManager::loadUsers() {
    vector<string> users;
    ifstream file("data/users.txt");

    if (!file) {
        // The file may not exist before the first registration.
        return users;
    }

    string line;

    while (getline(file, line)) {
        if (!line.empty())
            users.push_back(line);
    }

    return users;
}

bool FileManager::saveUser(string username, string password) {
    if (username.empty() || password.empty() ||
        username.find_first_of("|\r\n") != string::npos ||
        password.find_first_of("|\r\n") != string::npos) {
        cout << "Invalid username or password.\n";
        return false;
    }

    ofstream file("data/users.txt", ios::app);

    if (!file) {
        cout << "Could not open users.txt for writing.\n";
        return false;
    }

    file << username << '|' << password << '\n';
    file.close();

    if (!file) {
        cout << "Error saving user account.\n";
        return false;
    }

    return true;
}

vector<Order> FileManager::loadOrders() {
    vector<Order> orders;
    ifstream file("data/orders.txt");

    if (!file)
        return orders;

    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string id, customer, foodId, foodName;
        string price, quantity, status;

        if (!getline(ss, id, '|') ||
            !getline(ss, customer, '|') ||
            !getline(ss, foodId, '|') ||
            !getline(ss, foodName, '|') ||
            !getline(ss, price, '|') ||
            !getline(ss, quantity, '|') ||
            !getline(ss, status, '|')) {
            cout << "Skipping invalid order record.\n";
            continue;
        }

        try {
            int orderId = stoi(id);
            int fid = stoi(foodId);
            int p = stoi(price);
            int q = stoi(quantity);

            if (orderId <= 0 || fid <= 0 || p < 0 ||
                q <= 0 || customer.empty() ||
                customer.find('|') != string::npos ||
                foodName.empty() ||
                foodName.find('|') != string::npos ||
                status.find('|') != string::npos) {
                continue;
            }

            Order* existingOrder = nullptr;

            for (auto& order : orders) {
                if (order.getOrderId() == orderId &&
                    order.getCustomerName() == customer) {
                    existingOrder = &order;
                    break;
                }
            }

            if (existingOrder == nullptr) {
                orders.emplace_back(orderId, customer);
                existingOrder = &orders.back();
                existingOrder->setStatus(status);
            }

            FoodItem food(fid, foodName, p);
            existingOrder->addItem(food, q);

        } catch (...) {
            cout << "Skipping invalid order record.\n";
        }
    }

    return orders;
}

bool FileManager::saveOrders(const vector<Order>& orders) {
    // Validate before opening the file, so invalid data does not
    // immediately erase the previously saved order file.
    for (const auto& order : orders) {
        if (order.getCustomerName().empty() ||
            order.getCustomerName().find_first_of("|\r\n") != string::npos ||
            order.getStatus().empty() ||
            order.getStatus().find_first_of("|\r\n") != string::npos) {
            cout << "Invalid order data. Nothing was saved.\n";
            return false;
        }

        for (const auto& item : order.getItems()) {
            if (item.foodName.empty() ||
                item.foodName.find_first_of("|\r\n") != string::npos ||
                item.foodId <= 0 || item.price < 0 ||
                item.quantity <= 0) {
                cout << "Invalid order item. Nothing was saved.\n";
                return false;
            }
        }
    }

    ofstream file("data/orders.txt");

    if (!file) {
        cout << "Could not open orders.txt for writing.\n";
        return false;
    }

    for (const auto& order : orders) {
        for (const auto& item : order.getItems()) {
            file << order.getOrderId() << '|'
                 << order.getCustomerName() << '|'
                 << item.foodId << '|'
                 << item.foodName << '|'
                 << item.price << '|'
                 << item.quantity << '|'
                 << order.getStatus() << '\n';
        }
    }

    file.close();

    if (!file) {
        cout << "Error saving orders.\n";
        return false;
    }

    return true;
}
