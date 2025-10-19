#include <print>
#include <cstdint>
#include <span>
#include <iomanip>
#include <string_view>

struct Base {
  virtual ~Base() {
    std::print(" Base::~Base()");
  }
  virtual void f() {
    std::print("Base::f()");
  }
  virtual void g() {
    std::print("Base::g()");
  }
};

struct Derived : Base {
  ~Derived() {
    std::print("Derived::~Derived()");
  }
  virtual void f() override {
    std::print("Derived::f()");
  }
  virtual void h() {
    std::print("Derived::h()");
  }
};

void dumpVTable(void* vptr, void* obj) {
  std::println("vtable pointer (vptr): {:p}", vptr);
  std::println();

  // interpret the vptr as a pointer to an array of function pointers
  auto vtable = reinterpret_cast<void**>(vptr);

  using FnPtr = void(*)(void*);

  std::println(" i     vtable[i]         meaning");
  std::println("---- -------------  ----------------");

  std::println("[-2] {:p}             offset-to-top", vtable[-2]);
  std::println("[-1] {:p}  typeinfo (RTTI)", vtable[-1]);
  std::println("[ 0] {:p}  complete object destructor", vtable[0]);
  std::println("[ 1] {:p}  deleting destructor", vtable[1]);

  std::println();
  std::println(" i     vtable[i]     calling funtion");
  std::println("---- -------------  ----------------");

  for (size_t i {2}; i < 5; i++) {
    std::print("[ {}] {:p}  ", i, vtable[i]);

    if (vtable[i] != nullptr) {
      auto fn = reinterpret_cast<FnPtr>(vtable[i]);
      fn(obj);
    }
    std::println();
  }
}

int main() {
  Derived derived {};

  void* vptr = *(void**)&derived;

  dumpVTable(vptr, &derived);
  std::println();

  return 0;
}
