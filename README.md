# disa

**disa** is a simple C to RV32I assembly code compiler.

## Project Structure

- `input/`  
  Contains example `.c` source files that can be compiled.

- `src/`  
  Contains the core implementation.

- `test/`  
  Contains the testing suite, including test runners and individual test files to verify various functionalities.

## Usage

1. **Build the compiler**

    ```bash
    make
    ```

2. **Run the compiler**

    Use the run target with the ARGS variable to specify the input C file you want to compile

    ```bash
    make run ARGS="your_file.c"
    ```

    This will run the compiler on `input/your_file.c`.