
#include <iostream>
#include <vector>
#include <string>
#include <limits>

#include "FoodItem.h"
#include "User.h"
#include "Order.h"
#include "FileManager.h"

using namespace std;

// Function template: Unit 5
template <class T>
void showHeading(T title) {
    cout << "\n========== " << title << " ==========\n";
}

// Reads an integer within a given range.
int readNumber(string message, int minimum, int maximum) {
    int number;

    while (true) {
        cout << message;

        if (cin >> number &&
            number >= minimum && number <= maximum) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return number;
        }

        cout << "Invalid input. Try again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int main() {
    FileManager fm;

    // Load previously saved information.
    vector<FoodItem> menu = fm.loadMenu();
    vector<string> users = fm.loadUsers();
    vector<Order> orders = fm.loadOrders();

    // Add the initial menu if the menu file is empty.
    if (menu.empty()) {
        menu.push_back(FoodItem(101, "Burger", 100));
        menu.push_back(FoodItem(102, "Pizza", 200));
        menu.push_back(FoodItem(103, "Sandwich", 80));
        menu.push_back(FoodItem(104, "Biryani", 150));
        menu.push_back(FoodItem(105, "Cold Coffee", 60));

        if (!fm.saveMenu(menu)) {
            cout << "Menu could not be saved. Check data/menu.txt.\n";
            return 1;
        }
    }

    // Generate the next order ID.
    int nextOrderId = 1;

    for (const auto& order : orders) {
        if (order.getOrderId() >= nextOrderId)
            nextOrderId = order.getOrderId() + 1;
    }

    int choice;

    do {
        showHeading("ONLINE FOOD ORDERING SYSTEM");
        cout << "1. Customer Registration\n";
        cout << "2. Customer Login\n";
        cout << "3. Admin Login\n";
        cout << "4. Exit\n";

        choice = readNumber("Enter choice: ", 1, 4);

        // ------------------------------------------------
        // CUSTOMER REGISTRATION
        // ------------------------------------------------
        if (choice == 1) {
            string username, password;

            cout << "Choose username: ";
            getline(cin, username);

            cout << "Choose password: ";
            getline(cin, password);

            bool exists = false;

            for (const auto& record : users) {
                size_t separator = record.find('|');

                if (separator != string::npos &&
                    record.substr(0, separator) == username) {
                    exists = true;
                    break;
                }
            }

            if (username.empty() ||
                username.find_first_of("|;\r\n") != string::npos ||
                password.empty() ||
                password.find_first_of("|\r\n") != string::npos) {
                cout << "Invalid username or password.\n";
            }
            else if (exists) {
                cout << "Username already exists.\n";
            }
            else if (fm.saveUser(username, password)) {
                users.push_back(username + "|" + password);
                cout << "Registration successful!\n";
            }
        }

        // ------------------------------------------------
        // CUSTOMER LOGIN
        // ------------------------------------------------
        else if (choice == 2) {
            string username, password;

            cout << "Username: ";
            getline(cin, username);

            cout << "Password: ";
            getline(cin, password);

            bool loggedIn = false;

            for (const auto& record : users) {
                size_t separator = record.find('|');

                if (separator != string::npos &&
                    record.substr(0, separator) == username &&
                    record.substr(separator + 1) == password) {
                    loggedIn = true;
                    break;
                }
            }

            if (!loggedIn) {
                cout << "Incorrect username or password.\n";
                continue;
            }

            Customer customer(username);
            User* user = &customer;
            user->showRole();

            int option;

            do {
                showHeading("CUSTOMER MENU");
                cout << "1. View Menu\n";
                cout << "2. Place Order\n";
                cout << "3. View Order History\n";
                cout << "4. Track Order\n";
                cout << "5. Back\n";

                option = readNumber("Enter choice: ", 1, 5);

                // VIEW MENU
                if (option == 1) {
                    for (const auto& food : menu)
                        cout << food << '\n';
                }

                // PLACE A MULTI-ITEM ORDER
                else if (option == 2) {
                    Order order(nextOrderId, username);
                    int addMore = 1;

                    while (addMore == 1) {
                        cout << "\nAvailable Food:\n";

                        for (const auto& food : menu)
                            cout << food << '\n';

                        int id = readNumber("Enter food ID: ", 1, 1000000);
                        int quantity = readNumber(
                            "Enter quantity: ", 1, 100);

                        bool found = false;

                        for (const auto& food : menu) {
                            if (food.getId() == id) {
                                order.addItem(food, quantity);
                                found = true;
                                break;
                            }
                        }

                        if (!found) {
                            cout << "Food ID not found.\n";
                        } else {
                            cout << "Current bill: Rs."
                                 << order.calculateBill() << '\n';
                        }

                        addMore = readNumber(
                            "Add another item? (1=Yes, 0=No): ", 0, 1);
                    }

                    if (order.getItems().empty()) {
                        cout << "No items added to the order.\n";
                        continue;
                    }

                    order.display();

                    // Demonstrate function overloading.
                    cout << "Example bill after a 10% discount: Rs."
                         << order.calculateBill(10) << '\n';

                    int confirm = readNumber(
                        "Confirm order? (1=Yes, 0=No): ", 0, 1);

                    if (confirm == 1) {
                        orders.push_back(order);

                        if (fm.saveOrders(orders)) {
                            cout << "Order placed successfully!\n";
                            cout << "Your order ID: " << nextOrderId << '\n';
                            nextOrderId++;
                        } else {
                            orders.pop_back();
                            cout << "Order was not saved. Please try again.\n";
                        }
                    } else {
                        cout << "Order cancelled before placement.\n";
                    }
                }

                // ORDER HISTORY
                else if (option == 3) {
                    bool found = false;

                    for (const auto& order : orders) {
                        if (order.getCustomerName() == username) {
                            order.display();
                            found = true;
                        }
                    }

                    if (!found)
                        cout << "No previous orders.\n";
                }

                // TRACK ORDER
                else if (option == 4) {
                    int id = readNumber(
                        "Enter order ID: ", 1, 1000000);

                    bool found = false;

                    for (const auto& order : orders) {
                        if (order.getOrderId() == id &&
                            order.getCustomerName() == username) {
                            cout << "Order status: "
                                 << order.getStatus() << '\n';
                            found = true;
                            break;
                        }
                    }

                    if (!found)
                        cout << "Order not found for your account.\n";
                }

            } while (option != 5);
        }

        // ------------------------------------------------
        // ADMIN LOGIN
        // ------------------------------------------------
        else if (choice == 3) {
            string password;

            cout << "Enter admin password: ";
            getline(cin, password);

            if (password != "admin123") {
                cout << "Incorrect admin password.\n";
                continue;
            }

            Admin admin("Admin");
            User* user = &admin;
            user->showRole();

            int option;

            do {
                showHeading("ADMIN MENU");
                cout << "1. View Menu\n";
                cout << "2. Add Food Item\n";
                cout << "3. Update Food Price\n";
                cout << "4. View All Orders\n";
                cout << "5. Update Order Status\n";
                cout << "6. Back\n";

                option = readNumber("Enter choice: ", 1, 6);

                // VIEW MENU
                if (option == 1) {
                    for (const auto& food : menu)
                        cout << food << '\n';
                }

                // ADD FOOD
                else if (option == 2) {
                    int id = readNumber(
                        "Enter new food ID: ", 1, 1000000);

                    bool exists = false;

                    for (const auto& food : menu) {
                        if (food.getId() == id)
                            exists = true;
                    }

                    if (exists) {
                        cout << "Food ID already exists.\n";
                        continue;
                    }

                    string name;

                    cout << "Enter food name: ";
                    getline(cin, name);

                    int price = readNumber(
                        "Enter price in rupees: ", 0, 1000000);

                    if (name.empty() ||
                        name.find_first_of("|\r\n") != string::npos) {
                        cout << "Invalid food name.\n";
                        continue;
                    }

                    menu.push_back(FoodItem(id, name, price));

                    if (fm.saveMenu(menu)) {
                        cout << "Food item added successfully.\n";
                    } else {
                        menu.pop_back();
                        cout << "Food item could not be saved.\n";
                    }
                }

                // UPDATE PRICE
                else if (option == 3) {
                    int id = readNumber("Enter food ID: ", 1, 1000000);
                    int price = readNumber(
                        "Enter new price: ", 0, 1000000);

                    bool found = false;

                    for (auto& food : menu) {
                        if (food.getId() == id) {
                            int oldPrice = food.getPrice();
                            food.setPrice(price);

                            if (fm.saveMenu(menu)) {
                                cout << "Price updated successfully.\n";
                            } else {
                                food.setPrice(oldPrice);
                                cout << "Price could not be saved.\n";
                            }

                            found = true;
                            break;
                        }
                    }

                    if (!found)
                        cout << "Food ID not found.\n";
                }

                // VIEW ALL ORDERS
                else if (option == 4) {
                    if (orders.empty())
                        cout << "No orders available.\n";

                    for (const auto& order : orders)
                        order.display();
                }

                // UPDATE ORDER STATUS
                else if (option == 5) {
                    int id = readNumber("Enter order ID: ", 1, 1000000);

                    cout << "1. Placed\n";
                    cout << "2. Preparing\n";
                    cout << "3. Delivered\n";
                    cout << "4. Cancelled\n";

                    int statusChoice = readNumber(
                        "Choose new status: ", 1, 4);

                    string status;

                    if (statusChoice == 1)
                        status = "Placed";
                    else if (statusChoice == 2)
                        status = "Preparing";
                    else if (statusChoice == 3)
                        status = "Delivered";
                    else
                        status = "Cancelled";

                    bool found = false;

                    for (auto& order : orders) {
                        if (order.getOrderId() == id) {
                            string oldStatus = order.getStatus();
                            order.setStatus(status);

                            if (fm.saveOrders(orders)) {
                                cout << "Order status updated.\n";
                            } else {
                                order.setStatus(oldStatus);
                                cout << "Status could not be saved.\n";
                            }

                            found = true;
                            break;
                        }
                    }

                    if (!found)
                        cout << "Order ID not found.\n";
                }

            } while (option != 6);
        }

    } while (choice != 4);

    cout << "Thank you for using the system!\n";
    return 0;
}
