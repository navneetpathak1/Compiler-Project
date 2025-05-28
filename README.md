# Gehu Programming Language Compiler

## 🚀 Overview

Gehu is a cutting-edge programming language specifically crafted for scientific computing, engineering applications, and mathematical modeling. It combines the performance of compiled languages with the expressiveness needed for complex computational tasks, featuring built-in support for physical units, advanced pattern matching, and native concurrency primitives.

### Why Gehu?

- **Unit-Aware Computing**: Built-in support for physical units prevents dimensional analysis errors
- **Performance-First**: LLVM-based compilation ensures optimal performance
- **Concurrent by Design**: Native support for parallel computation and asynchronous operations
- **Type Safety**: Strong static typing with intelligent type inference
- **Functional Programming**: Pipeline operators and pattern matching for clean, readable code

## ✨ Key Features

### 🔢 Comprehensive Unit System
```gehu
let distance = 5 km;
let time = 30 min;
let speed = distance / time;  // Result: 10 km/h (automatic unit conversion)
let force = 50 kg * 9.81 m/s²;  // Result: 490.5 N
```

### 🔗 Pipeline Operator
```gehu
let result = data
    |> filter(is_positive)
    |> map(square)
    |> reduce(add);

let processed = 100 km
    |> convert_to(miles)
    |> calculate_travel_time(_, 65 mph);
```

### 🧵 Built-in Concurrency
```gehu
let task1 = spawn {
    let computation = heavy_calculation(data_set);
    return computation;
};

let task2 = spawn {
    let analysis = statistical_analysis(another_set);
    return analysis;
};

let results = await [task1, task2];
```

### 🎯 Pattern Matching
```gehu
match measurement {
    Distance(d) if d > 100 km => show "Long distance: " + d,
    Distance(d) => show "Short distance: " + d,
    Time(t) => show "Time measurement: " + t,
    _ => show "Unknown measurement type"
}
```

### 📊 Advanced Type System
```gehu
type Vector3D = {
    x: float m,
    y: float m,
    z: float m
};

type Result<T, E> = Success(T) | Error(E);

fn safe_divide(a: float, b: float) -> Result<float, string> {
    if b == 0.0 {
        return Error("Division by zero");
    }
    return Success(a / b);
}
```

## 🛠️ Building the Compiler

### Prerequisites

Before building Gehu, ensure you have the following dependencies installed:

- **CMake**: Version 3.15 or higher
- **C++ Compiler**: GCC 9+ or Clang 10+ with C++17 support
- **LLVM**: Version 12.0 or higher (with development headers)
- **Git**: For cloning the repository
- **Make**: Build system (or Ninja as alternative)

#### Installing Prerequisites

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install cmake build-essential llvm-12-dev libllvm12 llvm-12 git
```

**macOS (with Homebrew):**
```bash
brew install cmake llvm git
export PATH="/opt/homebrew/opt/llvm/bin:$PATH"
```

**Arch Linux:**
```bash
sudo pacman -S cmake clang llvm git base-devel
```

### Build Process

1. **Clone the Repository:**
```bash
git clone https://github.com/your-username/gehu.git
cd gehu
```

2. **Create Build Directory:**
```bash
mkdir build
cd build
```

3. **Configure with CMake:**
```bash
# Debug build (default)
cmake ..

# Release build (optimized)
cmake -DCMAKE_BUILD_TYPE=Release ..

# With custom LLVM path
cmake -DLLVM_DIR=/path/to/llvm/lib/cmake/llvm ..
```

4. **Compile the Project:**
```bash
# Using make
make -j$(nproc)

# Using ninja (if configured)
ninja
```

5. **Install (Optional):**
```bash
sudo make install
```

### Build Options

| Option | Description | Default |
|--------|-------------|---------|
| `CMAKE_BUILD_TYPE` | Build type (Debug/Release/RelWithDebInfo) | Debug |
| `GEHU_BUILD_TESTS` | Build unit tests | ON |
| `GEHU_BUILD_DOCS` | Build documentation | OFF |
| `GEHU_ENABLE_PROFILING` | Enable profiling support | OFF |

## 🚀 Usage

### Basic Compilation

```bash
# Compile a Gehu program
./gehu hello.gehu

# Specify output file
./gehu hello.gehu -o hello_world

# Enable optimization
./gehu hello.gehu -O2

# Debug information
./gehu hello.gehu -g
```

### Command Line Options

```
Usage: gehu [options] <input-file>

Options:
  -o <file>           Specify output file
  -O<level>          Optimization level (0-3)
  -g                 Include debug information
  -v, --verbose      Verbose output
  -h, --help         Show this help message
  --version          Show version information
  --emit-llvm        Output LLVM IR instead of executable
  --emit-ast         Output Abstract Syntax Tree
  --check            Check syntax without compilation
```

### Example Programs

**Hello World:**
```gehu
let greeting = "Hello, World!";
show greeting;
```

**Scientific Calculation:**
```gehu
// Physics calculation
let mass = 2.5 kg;
let acceleration = 9.81 m/s²;
let force = mass * acceleration;
show "Force: " + force;  // Output: Force: 24.525 N

// Unit conversions
let distance_km = 100 km;
let distance_miles = distance_km |> convert_to(miles);
show distance_miles;  // Output: 62.137 miles
```

**Concurrent Processing:**
```gehu
fn fibonacci(n: int) -> int {
    if n <= 1 { return n; }
    return fibonacci(n-1) + fibonacci(n-2);
}

let tasks = [];
for i in 1..10 {
    tasks.push(spawn { fibonacci(i) });
}

let results = await tasks;
show "Fibonacci results: " + results;
```

## 🧪 Testing

### Running Tests

```bash
# Run all tests
cd build
make test

# Run specific test categories
ctest -R lexer    # Lexer tests only
ctest -R parser   # Parser tests only
ctest -R codegen  # Code generation tests

# Verbose test output
ctest --verbose

# Run tests in parallel
ctest -j$(nproc)
```

### Test Categories

- **Unit Tests**: Individual component testing
- **Integration Tests**: End-to-end compilation tests
- **Performance Tests**: Benchmark comparisons
- **Language Tests**: Feature-specific language tests

## 📁 Project Architecture

```
gehu/
├── include/                    # Public header files
│   ├── gehu/
│   │   ├── lexer.hpp          # Lexical analyzer interface
│   │   ├── parser.hpp         # Parser interface
│   │   ├── semantic_analyzer.hpp # Semantic analysis
│   │   ├── code_generator.hpp # LLVM code generation
│   │   └── ast/               # AST node definitions
│   └── utils/                 # Utility headers
├── src/                       # Source implementation
│   ├── main.cpp              # Compiler entry point
│   ├── lexer/                # Lexical analysis
│   │   ├── lexer.cpp
│   │   ├── token.cpp
│   │   └── source_location.cpp
│   ├── parser/               # Syntax analysis
│   │   ├── parser.cpp
│   │   ├── ast.cpp
│   │   └── expression_parser.cpp
│   ├── semantic/             # Semantic analysis
│   │   ├── semantic_analyzer.cpp
│   │   ├── type_checker.cpp
│   │   ├── symbol_table.cpp
│   │   └── unit_system.cpp
│   ├── codegen/              # Code generation
│   │   ├── llvm_codegen.cpp
│   │   ├── optimization_passes.cpp
│   │   └── runtime_support.cpp
│   └── utils/                # Utility implementations
├── test/                     # Test suite
│   ├── unit/                 # Unit tests
│   ├── integration/          # Integration tests
│   ├── benchmark/            # Performance tests
│   └── examples/             # Example programs
├── docs/                     # Documentation
│   ├── language_reference.md
│   ├── api_reference.md
│   └── tutorials/
├── scripts/                  # Build and utility scripts
├── cmake/                    # CMake modules
└── tools/                    # Additional tools
```

## 📚 Language Reference

### Type System

Gehu features a sophisticated type system with the following built-in types:

#### Primitive Types
- `int`: 64-bit signed integer
- `float`: 64-bit floating point
- `bool`: Boolean (true/false)
- `string`: UTF-8 string
- `char`: Unicode character

#### Unit Types
```gehu
// Length units
let distance: float m = 100.0;
let height: float ft = 6.2;

// Time units
let duration: float s = 30.5;
let interval: float min = 5.0;

// Derived units
let speed: float m/s = distance / duration;
let acceleration: float m/s² = 9.81;
```

#### Collection Types
```gehu
// Arrays
let numbers: [int] = [1, 2, 3, 4, 5];
let measurements: [float m] = [1.5 m, 2.3 m, 0.8 m];

// Tuples
let point: (float, float) = (3.14, 2.71);
let measurement: (float m, float s) = (100.0 m, 10.5 s);
```

### Control Flow

#### Conditionals
```gehu
if temperature > 100.0 °C {
    show "Water is boiling";
} else if temperature < 0.0 °C {
    show "Water is frozen";
} else {
    show "Water is liquid";
}
```

#### Loops
```gehu
// For loops
for i in 0..10 {
    show i;
}

for measurement in sensor_data {
    process(measurement);
}

// While loops
while distance < target {
    distance += step_size;
    update_position(distance);
}
```

### Functions

```gehu
fn calculate_kinetic_energy(mass: float kg, velocity: float m/s) -> float J {
    return 0.5 * mass * velocity²;
}

fn generic_maximum<T>(a: T, b: T) -> T where T: Comparable {
    if a > b { a } else { b }
}
```

### Error Handling

```gehu
type Result<T, E> = Ok(T) | Err(E);

fn safe_sqrt(x: float) -> Result<float, string> {
    if x < 0.0 {
        return Err("Cannot take square root of negative number");
    }
    return Ok(sqrt(x));
}

match safe_sqrt(-4.0) {
    Ok(result) => show "Result: " + result,
    Err(error) => show "Error: " + error
}
```

## 🤝 Contributing

We welcome contributions to the Gehu programming language! Here's how you can help:

### Development Setup

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/amazing-feature`
3. Install development dependencies:
```bash
# Install pre-commit hooks
pip install pre-commit
pre-commit install

# Install clang-format for code formatting
sudo apt install clang-format  # Ubuntu/Debian
```

### Contribution Guidelines

- **Code Style**: Follow the existing code style (enforced by clang-format)
- **Testing**: Add tests for new features
- **Documentation**: Update documentation for user-facing changes
- **Commit Messages**: Use conventional commit format

### Areas for Contribution

- 🐛 **Bug Fixes**: Help us squash bugs
- 🚀 **Performance**: Optimization opportunities
- 📚 **Documentation**: Improve examples and guides
- 🧪 **Testing**: Expand test coverage
- 🌟 **Features**: Implement new language features

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

```
MIT License

Copyright (c) 2024 Gehu Contributors

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## 🙏 Acknowledgments

- **LLVM Project**: For the excellent compiler infrastructure
- **Scientific Computing Community**: For inspiration and requirements
- **Contributors**: Everyone who has contributed to making Gehu better

## 📞 Support & Community

- 🐛 **Bug Reports**: [GitHub Issues](https://github.com/your-username/gehu/issues)
- 💬 **Discussions**: [GitHub Discussions](https://github.com/your-username/gehu/discussions)
- 📧 **Email**: gehu-dev@example.com
- 🌐 **Website**: [https://gehu-lang.org](https://gehu-lang.org)

---
