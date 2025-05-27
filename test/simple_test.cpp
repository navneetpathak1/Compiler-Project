#include <iostream>

// Function to add two numbers
double add(double a, double b) {
    return a + b;
}

// Function to show a value
void show(double value) {
    std::cout << "Result: " << value << std::endl;
}

int main() {
    // Declare and initialize x
    double x = 42;
    
    // Call add function and store result
    double result = add(x, 10);
    
    // Show the result
    show(result);
    
    return 0;
} 