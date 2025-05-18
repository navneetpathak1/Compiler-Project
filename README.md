# Compiler-Project

This repository contains a compiler designed in **C** as part of the **Compiler Design course** at **GEHU Bhimtal**, under the guidance of **Devesh Pandey Sir**.

The compiler parses input files according to specified language rules. If the input is syntactically and semantically correct, the compiler generates corresponding **assembly-level code**. Test cases are provided in the `_Compiler/testcases_` directory.

> ⚠️ The compiler is compatible with **GCC 5.4.0** and has been tested on **Ubuntu 16.04**.

---

## 🛠️ How to Build the Compiler

1. Navigate to the compiler directory:

   ```bash
   cd Compiler
   ```
2. Build the executable:

   ```bash
   make
   ```
3. This will create an executable file named `compiler`.

---

## ▶️ How to Run the Compiler on a Test Case

1. Ensure you are in the `Compiler/` directory.
2. Run the compiler:

   ```bash
   ./compiler <RELATIVE_PATH_TO_TESTCASE_FILE> <NAME_OF_ASM_FILE>
   ```

   Example:

   ```bash
   ./compiler testcases/testcase0.txt code.asm
   ```
3. A menu with 11 options will appear. Choose an option to test a specific compiler phase.
4. To generate assembly-level code, select option `10`.
5. Use option `0` to exit the menu.
   ⚠️ Pressing `Ctrl+C` exits without saving the `.asm` output.
6. To compile and run the generated assembly:

   ```bash
   nasm -felf64 code.asm && gcc code.o && ./a.out
   ```

---

## 🔁 Compilation Stages (Triggered via Option 10)

| Stage                 | Description                                                                                                                       |
| --------------------- | --------------------------------------------------------------------------------------------------------------------------------- |
| **Lexical Analysis**  | Breaks the source into tokens as per language specifications.                                                                     |
| **Syntax Analysis**   | Constructs a Parse Tree from tokens. Errors here prevent further processing.                                                      |
| **Semantic Analysis** | Builds an AST, performs type checking, and populates the Symbol Table. Errors here prevent code generation.                       |
| **Code Generation**   | Converts the AST to assembly code. ⚠️ Code generation supports only programs with a single `main` function using `int` variables. |

---

## 👨‍💼 Contributors

* [Navneet Pathak](https://github.com/navneetpathak1) and Team
