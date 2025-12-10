#include <iostream>
#include <string>

class Human {
public:
    virtual ~Human() = default;
    virtual std::string GetPhrase() { return "I have nothing"; }
};

class Driver : virtual public Human {
public:
    virtual std::string GetPhrase() { return std::string("I have ") + car; }

protected:
    std::string car{"autobot"};
};

class Potter : virtual public Human {
public:
    virtual std::string GetPhrase() { return std::string("I have ") + pot; }

protected:
    std::string pot{"ampula"};
};

class DriverPotter : public Driver, public Potter {
public:
    std::string GetPhrase() { return std::string("I have ") + car + " and " + pot; }
};

int main() {
    Human* human = new Human;
    std::cout << human->GetPhrase() << std::endl;
    delete human;
    human = new Driver;
    std::cout << human->GetPhrase() << std::endl;
    delete human;
    human = new Potter;
    std::cout << human->GetPhrase() << std::endl;
    delete human;
    human = new DriverPotter;
    std::cout << human->GetPhrase() << std::endl;
    delete human;
    return 0;
}
