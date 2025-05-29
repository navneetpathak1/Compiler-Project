# Gehu Language Compiler

**Gehu** is a custom-built programming language that includes a fully functional compiler implemented in modern C++ using the LLVM framework. This project demonstrates all phases of compiler construction — from lexical analysis to native code generation — and serves as an educational tool for understanding compiler design.

---

## 📌 Features

- Full compiler pipeline:
  - Lexical Analysis
  - Syntax Parsing (Recursive Descent)
  - Semantic Analysis (Type Checking, Scoping)
  - LLVM-based Code Generation
- Expression evaluation, control flow, and variable management
- `show` command for output
- Sample `.gehu` programs included for testing and demonstration
- Modular, extensible codebase


### 🧵 Built-in Concurrency
```gehu
let task1 = spawn {


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

## 👨‍💻 Authors

Developed by [Navneet Pathak](https://github.com/navneetpathak1) and team.
