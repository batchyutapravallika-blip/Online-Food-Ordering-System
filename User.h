
#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
using namespace std;

class User {
protected:
    string name;

public:
    User(string n);
    virtual void showRole() = 0;
    virtual ~User();
};

class Customer : public User {
public:
    Customer(string n);
    void showRole() override;
};

class Admin : public User {
public:
    Admin(string n);
    void showRole() override;
};

#endif
