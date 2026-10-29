
#include "User.h"

User::User(string n) {
    name = n;
}

User::~User() {
}

Customer::Customer(string n) : User(n) {
}

void Customer::showRole() {
    cout << "Customer: " << name << endl;
}

Admin::Admin(string n) : User(n) {
}

void Admin::showRole() {
    cout << "Administrator: " << name << endl;
}
