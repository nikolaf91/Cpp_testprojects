// BigInt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "BigInt.h"

int main()
{

    // Declare some big integers for testing
    CBigInt a, b, c;
    // Assign some values to them
    a = CBigInt(144);
    b = CBigInt(5);

    // Test the multiply function
    c = a * b;
    // Print the result in hexadecimal
    printf("a * b = %ls\n", c.ToHex().c_str());
    a *= a;
    //Print the result in hexadecimal
    printf("a * a = %ls\n", a.ToHex().c_str());
  
    // Test the divide function
    try {
        c = a / b;
        // Print the result in hexadecimal
        printf("a / b = %ls\n", c.ToHex().c_str());
    } catch (const std::runtime_error& e) {
        // Print the error message
        std::cerr << "Caught runtime error1: " << e.what() << '\n';
    }

    try {
        c = b / a;
        // Print the result in hexadecimal
        printf("b / a = %ls\n", c.ToHex().c_str());
    } catch (const std::runtime_error& e) {
        // Print the error message
        std::cerr << "Caught runtime error2: " << e.what() << '\n';
    }
    
    //Test the modulo function
    try {
        c = a % b;
        // Print the result in hexadecimal
        printf("a %% b = %ls\n", c.ToHex().c_str());
    } catch (const std::runtime_error& e) {
        // Print the error message
        std::cerr << "Caught runtime error3: " << e.what() << '\n';
    }

    try {
        c = b % a;
        // Print the result in hexadecimal
        printf("b %% a = %ls\n", c.ToHex().c_str());
    } catch (const std::runtime_error& e) {
        // Print the error message
        std::cerr << "Caught runtime error4: " << e.what() << '\n';
    }

    // Test the subtract function
    try {
        c = a - b;
        // Print the result in hexadecimal
        printf("a - b = %ls\n", c.ToHex().c_str());
    } catch (const std::runtime_error& e) {
        // Print the error message
        std::cerr << "Caught runtime error2: " << e.what() << '\n';
    }

    // Test the add function
    c = a + b;
    // Print the result in hexadecimal
    printf("a + b = %ls\n", c.ToHex().c_str());

    return 0;
}
