# Rectangle Intersection Finder

This C++17 console application takes a JSON file containing a list of rectangles, computes all intersections (including 2-way, 3-way, ..., N-way), and prints both the original rectangles and the intersections to standard output.

---

## 📦 Features

- Parses a JSON file with rectangle definitions
- Detects and reports all overlapping regions between any two or more rectangles
- Supports recursive intersection detection
- Validates input format and dimensions
- Processing limited to the first 10 rectangles
- Uses JSON parser from [nlohmann/json](https://github.com/nlohmann/json)

---

## 📂 Input Format

Input JSON file should look like this:

```json
{
  "rects": [
    {"x": 100, "y": 100, "w": 250, "h": 80 },
    {"x": 120, "y": 200, "w": 250, "h": 150 },
    {"x": 140, "y": 160, "w": 250, "h": 100 },
    {"x": 160, "y": 140, "w": 350, "h": 190 }
  ]
}
```

---

## 🛠️ Building the Project

Requires:
- CMake ≥ 3.8
- A C++17-compatible compiler (e.g., GCC, Clang, MSVC)

### 🔧 Build (single command)

```bash
cmake -S . -B build && cmake --build build
```

This creates the `build/` directory and compiles the `intersection_finder` executable inside it.

---

## 🚀 Running the Program

```bash
./build/intersection_finder path/to/rectangles.json
```

Example:
```bash
./build/intersection_finder test_rects.json
```

The program will output:
- A list of all valid input rectangles
- A list of all computed intersections, grouped by participating rectangle IDs

---

## 🧩 Notes

- Only the **first 10 rectangles** in the JSON file are processed
- Zero-size rectangles are ignored
- Negative dimensions will cause an error
- Duplicate rectangles are treated as distinct entities