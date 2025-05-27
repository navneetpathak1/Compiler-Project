# Gehu Programming Language Compiler

Gehu is a modern programming language designed for scientific and engineering computations, with built-in support for units, pattern matching, and concurrency.

## Features

- Strong type system with unit support
- Pattern matching
- Pipeline operator for functional programming
- Built-in concurrency primitives
- LLVM-based code generation

## Building

### Prerequisites

- CMake 3.10 or higher
- C++17 compatible compiler
- LLVM 10.0 or higher
- Clang (for compiling generated LLVM IR)

### Build Steps

1. Create a build directory:
```bash
mkdir build
cd build
```

2. Configure with CMake:
```bash
cmake ..
```

3. Build the project:
```bash
make
```

## Usage

### Compiling a Gehu Program

```bash
./gehu input.gehu
```

This will generate an executable named `output` in the current directory.

### Running Tests

```bash
cd build
make test
```

## Language Features

### Units

```gehu
let distance = 5 km;
let time = 30 min;
let speed = distance / time;  // Automatically handles unit conversion
```

### Pipeline Operator

```gehu
let result = 100 km |> calculate_speed(_, 2 min);
```

### Concurrency

```gehu
let task1 = spawn {
    let x = 10 km;
    show x;
};

wait task1;
```

## Project Structure

- `include/` - Header files
  - `lexer.hpp` - Lexical analyzer
  - `parser.hpp` - Parser
  - `semantic_analyzer.hpp` - Semantic analyzer
  - `code_generator.hpp` - LLVM code generator
- `source/` - Source files
  - `main.cpp` - Main compiler driver
  - `lexer/` - Lexical analysis components
    - `lexer.hpp` - Lexer interface
    - `lexer.cpp` - Lexer implementation
  - `parser/` - Parsing components
    - `ast.hpp` - Abstract Syntax Tree definitions
    - `ast.cpp` - AST implementation
  - `semantic/` - Semantic analysis components
    - `semantic_visitor.hpp` - Semantic visitor interface
    - `semantic_visitor.cpp` - Semantic visitor implementation
    - `type.hpp` - Type definitions
  - `codegen/` - Code generation components
    - `codegen_visitor.hpp` - Code generation visitor interface
    - `codegen_visitor.cpp` - Code generation visitor implementation
- `test/` - Test files
  - `test.cpp` - Unit tests
  - `test.gehu` - Sample Gehu program

## Grammar of the Language

The grammar of the Gehu language is defined by the tokens and parsing rules in `lexer.cpp` and `parser.cpp`. Here's a simplified overview:

- **Tokens:**
  - `Let`: Keyword for variable declaration.
  - `Identifier`: Variable names.
  - `Assign`: Assignment operator (`=`).
  - `String`: String literals enclosed in double quotes.
  - `Show`: Keyword for displaying variables.
  - `Semicolon`: Statement terminator (`;`).
  - `Eof`: End of file.

- **Statements:**
  - **Let Statement:**  
    ```
    let <identifier> = <string> ;
    ```
    Declares a variable and assigns a string value to it.

  - **Show Statement:**  
    ```
    show <identifier> ;
    ```
    Displays the value of a variable.

- **Example:**
  ```
  let name = "John";
  show name;
  ```

## License

This project is licensed under the MIT License - see the LICENSE file for details. 