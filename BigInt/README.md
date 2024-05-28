<h2>main.cpp:</h2>
The main.cpp file serves as a test suite for the CBigInt class, showcasing its ability to handle large integer arithmetic operations. <br>
Here are the key functionalities demonstrated:<br><br>


Multiplication: Squaring a number and multiplying two different numbers.<br>
Division: Dividing two numbers with error handling for division errors.<br>
Modulo: Calculating the remainder of division operations with error handling.<br>
Subtraction: Subtracting one large integer from another.<br>
Addition: Adding two large integers.<br>
Hexadecimal Output: Printing the results of operations in hexadecimal format using the ToHex method of the CBigInt class.

<h2>BigInt.cpp:</h2>
<b>Resource management:</b> <br>
Detach:Decrements the reference count of the current SBigInt object.<br>
If the reference count reaches zero, it deletes the associated data.<br>

Attach: Attaches the current object to a new SBigInt object by incrementing its reference count.<br>
Detaches from any previously attached object.<br>

<b>Data allocation:</b><br>

count: Calculates the number of BIGINT_CHUNK elements needed to store uSize bytes.<br>

AlocData: Allocates memory for a new SBigInt object of size uSize and attaches it to the current object.<br>

<b>Conversion methods:</b><br>
ToHex/ToHex: Converts the big integer/hexadecimal string to a hexadecimal string/big integer.<br>

ToBin/FromBin: Converts the big integer/binary string to a binary string/big integer.<br>

<b>Comparison Method:</b><br>
Compare: Compares two big integers and returns 0, 1, -1 if current object equal, larger or smaller than other object<br>

Arithmetic Operations Add, Subtract, Multiply, Divide and Module for 2 big integers operations






