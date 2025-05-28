Gehu Programming Language Compiler

Gehu is a modern, statically-typed programming language designed for scientific and engineering applications. It features a powerful type system with native unit support, expressive syntax for pattern matching, a pipeline operator for functional composition, and built-in primitives for concurrent programming. Gehu compiles to efficient machine code using LLVM.

⸻

🚀 Features
	•	Strong Type System with Units
Seamless integration of physical units into type checking and arithmetic.
	•	Pattern Matching
Concise and readable syntax for conditional logic based on structural patterns.
	•	Pipeline Operator (|>)
Functional programming support for cleaner and more composable code.
	•	Concurrency Primitives
Native support for lightweight, concurrent tasks.
	•	LLVM Backend
High-performance code generation using the LLVM toolchain.

⸻

🛠️ Building the Compiler

Prerequisites

To build Gehu, ensure you have the following tools installed:
	•	CMake ≥ 3.10
	•	C++ Compiler with C++17 support
	•	LLVM ≥ 10.0
	•	Clang (used to compile the generated LLVM IR)

🛠️ Building the Compiler

Prerequisites

To build Gehu, ensure you have the following tools installed:
	•	CMake ≥ 3.10
	•	C++ Compiler with C++17 support
	•	LLVM ≥ 10.0
	•	Clang (used to compile the generated LLVM IR)

Build Instructions
# Step 1: Create a build directory
mkdir build
cd build

# Step 2: Configure the project with CMake
cmake ..

# Step 3: Compile the source code
make

After a successful build, the gehu compiler binary will be available in the build directory.


📦 Usage

Compiling a Gehu Program
./gehu path/to/input.gehu

This will compile the input file and generate an executable named output in the current directory.

Running Tests

To execute the test suite:
cd build
make test

🧠 Language Features

Units

Gehu natively understands physical units and handles conversions automatically.
let distance = 5 km;
let time = 30 min;
let speed = distance / time;  // Units are managed automatically

Pipeline Operator

Used for function chaining and improving code readability:
let result = 100 km |> calculate_speed(_, 2 min);

Concurrency

Gehu supports lightweight concurrency with spawn and wait:
let task1 = spawn {
    let x = 10 km;
    show x;
};

wait task1;

.
├── include/                  # Header files
│   ├── lexer.hpp
│   ├── parser.hpp
│   ├── semantic_analyzer.hpp
│   └── code_generator.hpp
├── source/                   # Source code
│   ├── main.cpp              # Compiler driver
│   ├── lexer/                # Lexical analysis
│   │   ├── lexer.hpp
│   │   └── lexer.cpp
│   ├── parser/               # Parsing
│   │   ├── ast.hpp
│   │   └── ast.cpp
│   ├── semantic/             # Semantic analysis
│   │   ├── semantic_visitor.hpp
│   │   ├── semantic_visitor.cpp
│   │   └── type.hpp
│   └── codegen/              # Code generation
│       ├── codegen_visitor.hpp
│       └── codegen_visitor.cpp
├── test/                     # Unit and functional tests
│   ├── test.cpp
│   └── test.gehu
└── CMakeLists.txt            # CMake configuration

📚 Language Grammar

The Gehu language is defined by its token set and parsing rules located in lexer.cpp and parser.cpp.

Tokens
	•	let – variable declaration keyword
	•	identifier – variable names
	•	= – assignment operator
	•	string – double-quoted string literals
	•	show – output command
	•	; – statement terminator
	•	eof – end of file

Statement Forms
	•	Variable Declaration
        let <identifier> = <string> ;
	•	Output Statement
        let name = "John";
        show name;

📄 License

This project is licensed under the MIT License. You are free to use, modify, and distribute this software with proper attribution.
