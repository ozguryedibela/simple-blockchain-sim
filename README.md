# Blockchain-Based Exchange and Wallet Simulation

This project is a simple asset management and exchange simulation developed using C++. The primary objective is to utilize a fundamental Blockchain structure to ensure the security and tracking of financial transactions such as deposits, withdrawals, and coin trading.

Every transaction generates a unique block, and these blocks are linked together using cryptographic hashing (SHA-256), ensuring data integrity.

## Project Features

* **Blockchain Architecture:** Every transaction creates a new block. Each block contains the hash value of the previous block, maintaining the chain's integrity.
* **SHA-256 Hashing:** The SHA-256 algorithm is integrated to encrypt data and sign blocks.
* **Data Persistence:** User balance and coin assets are stored in `asset_information.txt`. Data remains persistent even after the program is terminated.
* **Transaction Logging:** All transactions are recorded in `transactions.log` along with their respective hash values.
* **Dynamic Balance Management:** Pointers are utilized to optimize memory management and data flow between functions.

## Technical Details

The following C++ concepts are actively used in this project:

* **Object-Oriented Programming (OOP):** Data abstraction using the `Block` class.
* **File I/O:** Reading and writing operations using the `fstream` library.
* **Pointers & Memory Management:** Passing data by reference to functions.
* **Vector:** Using `vector` to dynamically manage the chain structure.
* **Hashing:** External SHA-256 implementation.

## Installation and Compilation

To run this project, ensure that `sha256.h` and `sha256.cpp` are located in the same directory as the source code.
