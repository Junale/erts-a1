#include <iostream>
#include <string>

using namespace std;

template <typename T> class Singleton {
public:
  static T &getInstance() {
    static T instance;
    return instance;
  }

protected:
  Singleton() = default;
  ~Singleton() = default;

  Singleton(const Singleton &) = delete;
  Singleton &operator=(const Singleton &) = delete;
};

class Vehicle : public Singleton<Vehicle> {
  friend class Singleton<Vehicle>;

public:
  string model = "Vehicle";

private:
  Vehicle() = default;
};

class Car : public Vehicle {
  friend class Singleton<Car>;

public:
  string model = "Mustang";

private:
  Car() = default;
};

int main() {
  Vehicle &vehicle = Vehicle::getInstance();
  Vehicle &car = Car::getInstance();

  cout << vehicle.model << endl;
  cout << car.model << endl;

  return 0;
}