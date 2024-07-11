
# Loan Management System

This project is a Loan Management System developed in C++ as part of a File Structures course. The system uses a hashing indexing technique to efficiently store and retrieve loan-related information of customers. It demonstrates the practical application of file structures and hashing techniques in managing financial data.

## Features

- Add new loan records
- Search for loan records using customer information
- Update existing loan records
- Delete loan records
- Display all loan records
- Efficient data storage using hashing indexing technique

## Requirements

- C++ compiler (GCC, Clang, or any other standard - compiler)
- File handling capabilities

## How It Works

- Loan Class: Represents the loan information for a customer, including details such as customer ID, loan amount, interest rate, and tenure.

- HashTable Class: Implements a hash table to efficiently store and retrieve loan records. The hash table uses a hashing function to compute an index for each record.

- File Handling: The loan records are stored in files within the data/ directory. Each operation (add, search, update, delete) involves reading from and writing to these files.

- Hashing Technique: The hashing function computes an index based on the customer ID, ensuring efficient data retrieval and minimizing collisions.
