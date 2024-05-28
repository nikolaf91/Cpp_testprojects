<h2>main.cpp:</h2>
The main.cpp file serves as a test suite for the CBigInt class, showcasing its ability to handle large integer arithmetic operations. 
Here are the key functionalities demonstrated:

Multiplication: Squaring a number and multiplying two different numbers.<br>
Division: Dividing two numbers with error handling for division errors.<br>
Modulo: Calculating the remainder of division operations with error handling.<br>
Subtraction: Subtracting one large integer from another.<br>
Addition: Adding two large integers.<br>
Hexadecimal Output: Printing the results of operations in hexadecimal format using the ToHex method of the CBigInt class.<br>

<h2>BigInt.cpp:</h2>
Resource management: 
Detach:Decrements the reference count of the current SBigInt object.
If the reference count reaches zero, it deletes the associated data.

Attach: Attaches the current object to a new SBigInt object by incrementing its reference count.
Detaches from any previously attached object.

Data allocation:

count: Calculates the number of BIGINT_CHUNK elements needed to store uSize bytes.

AlocData: Allocates memory for a new SBigInt object of size uSize and attaches it to the current object.

Conversion methods:
ToHex/ToHex: Converts the big integer/hexadecimal string to a hexadecimal string/big integer.

ToBin/FromBin: Converts the big integer/binary string to a binary string/big integer.

Comparison Method:
Compare: Compares two big integers and returns 0, 1, -1 if current object equal, larger or smaller than other object

Arithmetic Operations Add, Subtract, Multiply, Divide and Module for 2 big integers operations






