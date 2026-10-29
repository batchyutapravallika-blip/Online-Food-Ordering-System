
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "FoodItem.h"
#include "Order.h"
#include <string>
#include <vector>
using namespace std;

class FileManager {
public:
    vector<FoodItem> loadMenu();
    bool saveMenu(const vector<FoodItem>& menu);

    vector<string> loadUsers();
    bool saveUser(string username, string password);

    vector<Order> loadOrders();
    bool saveOrders(const vector<Order>& orders);
};

#endif
