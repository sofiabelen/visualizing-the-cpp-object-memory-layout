
# Visualizing C++ Object Memory Layout

This repository contains small experiments to visualize how C++ objects are laid out in memory, with a focus on **vptr, vtable, object members, and memory padding**. It accompanies a series of blog posts exploring these concepts in depth.

Read the blog post (Part 1: Single Inheritance) here: [Visualizing the C++ Object Memory Layout Part 1: Single Inheritance](https://sofiabelen.github.io/projects/visualizing-the-cpp-object-memory-layout-part-1-single-inheritance/)

![](assets/vtable-both.svg)

---

## Project Structure

```
part_1_single_inheritance/
├─ vtable.cpp # Experiment with virtual functions and vtable
└─ object_layout.cpp # Experiment with object memory layout
CMakeLists.txt
```
---

## Building

This project uses **CMake** and **C++23**. You can build the executables in a separate `build` directory:

```bash
# Create a build directory
mkdir -p build
cd build

# Generate build files
cmake ..

# Build the project
cmake --build .

# Run the experiments
./vtable
./object_layout
```

## License

MIT
