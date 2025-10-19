#include <print>
#include <cstdint>
#include <span>
#include <iomanip>
#include <string_view>
#include <vector>

struct Base {
  Base(int b = 0) : b_data(b) {}

  virtual void f() {};
  int b_data;
};

struct Derived : Base {
  Derived(int b = 0, int d = 0)
    : Base(b), d_data(d) {}

  int d_data;
};

struct Field {
  std::size_t offset;
  std::size_t size;
  std::string_view name;
};

template <typename T>
std::vector<Field> addPadding(std::span<const Field> fields) {
  std::vector<Field> fieldsWithPadding;
  size_t prevEnd {};

  for (const auto& f: fields) {
    if (f.offset > prevEnd)
      fieldsWithPadding.emplace_back(
          Field { prevEnd, f.offset - prevEnd, "padding"}
          );
    fieldsWithPadding.emplace_back(f);
    prevEnd = f.offset + f.size;
  }

  if (prevEnd < sizeof(T))
    fieldsWithPadding.emplace_back(
        Field { prevEnd, sizeof(T) - prevEnd, "padding" }
        );

  return fieldsWithPadding;
}

template <typename T>
void dumpLabeled(std::span<const std::byte> dataBytes,
    std::span<const Field> fields) {
  std::println("Offset  Size  Field     Bytes");
  std::println("------  ----  -----     ------");
  auto fieldsWithPaddding = addPadding<T>(fields);

  for (const auto& field: fieldsWithPaddding) {
    std::print("0x{:02X}      {}    {:<8} ", field.offset, field.size, field.name);

    for (std::size_t i {}; i < field.size && (field.offset + i) < dataBytes.size(); i++) {
      auto b { dataBytes[field.offset + i] };
      std::print("{:02x} ", std::to_integer<unsigned>(b));
    }
    std::println();
  }
  std::println();
}

int main() {
  std::println("sizeof(Base):    {}", sizeof(Base));
  std::println("sizeof(Derived): {}", sizeof(Derived));
  std::println();

  Base base {1};
  Derived derived {2, 3};

  Base* basePtrToDerived = &derived;

  std::println("&derived (Derived*):          {:p}", static_cast<void*>(&derived));
  std::println("&base (Base*):                {:p}", static_cast<void*>(&base));
  std::println();

  auto derivedBytes { std::as_bytes(std::span{&derived, 1}) };
  std::uintptr_t derivedPtr { reinterpret_cast<std::uintptr_t>(&derived)};

  Field derivedFields [] = {
    {0x00, sizeof(void*), "vptr"},
    {reinterpret_cast<std::uintptr_t>(&derived.b_data) - derivedPtr, sizeof(derived.b_data), "b_data"},
    {reinterpret_cast<std::uintptr_t>(&derived.d_data) - derivedPtr, sizeof(derived.d_data), "d_data"}
  };

  auto baseBytes { std::as_bytes(std::span{&base, 1}) };
  std::uintptr_t basePtr { reinterpret_cast<std::uintptr_t>(&base)};

  std::vector<Field> baseFields = {
    {0x00, sizeof(void*), "vptr"},
    {reinterpret_cast<std::uintptr_t>(&base.b_data) - basePtr, sizeof(base.b_data), "b_data"}
  };

  std::println("Base obj bytes (annotated with padding):");
  dumpLabeled<Base>(baseBytes, baseFields);

  std::println("Derived obj bytes (annotated with padding):");
  dumpLabeled<Derived>(derivedBytes, derivedFields);

  auto baseOffset = reinterpret_cast<std::uintptr_t>(basePtrToDerived) -
    reinterpret_cast<std::uintptr_t>(&derived);

  std::println("Offet of Base within Derived: {} bytes", baseOffset);

  return 0;
}
