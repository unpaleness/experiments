#include <iostream>
#include <optional>

class A {
public:
  A() {}
  A(int new_data) : data(new_data) {}
  A(const A &other) : data(other.data) {}
  A(A &&other) : data(std::exchange(other.data, 0)) {}
  ~A() {}

  const A &operator=(int new_data) {
    data = new_data;
    return *this;
  }

  const A &operator=(const A &other) {
    data = other.data;
    return *this;
  }

  const A &operator=(A &&other) {
    data = std::exchange(other.data, 0);
    return *this;
  }

  friend std::ostream &operator<<(std::ostream &out, const A &a) {
    out << a.data;
    return out;
  }

private:
  int data;
};

void print(const char *name, const std::optional<A> &a) {
  std::cout << name << " = " << (a.has_value() ? a.value() : 0) << std::endl;
}

void print(const char *name, const A &a) {
  std::cout << name << " = " << a << std::endl;
}

int main() {
  std::optional<A> a;
  std::optional<A> b;
  print("a", a);
  a = 10;
  print("a", a);
  b = 20;
  print("b", b);
  print("a", a);
  b = std::move(a);
  print("b", b);
  print("a", a);
  a = A(25);
  print("a", a);
  A aa(30);
  A bb(40);
  print("aa", aa);
  print("bb", bb);
  bb = std::move(aa);
  print("aa", aa);
  print("bb", bb);
  return 0;
}
