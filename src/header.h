#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structures
struct Date {
    int day, month, year;
};

struct Record {
    int id;                 // Unique account identifier
    int userId;             // ID of the user who owns this account
    int accountNbr;         // Account number
    struct Date deposit;    // Deposit date
    char country[69];       // Country associated with the account
    int phone;              // Phone number associated with the account
    double amount;          // Amount of money in the account
    char accountType[69];   // Type of account (e.g., saving, current, fixed01)
    char username[69];
    struct Date withdraw;
};

struct User {
    int id;                 // Unique user identifier
    char name[69];          // Username
    char password[69];      // User password
};

// Function declarations

// Helper functions
double calculateInterest(struct Record r);
void displayInterest(struct Record r);

// Account management
int getAccountFromFile(FILE *ptr, char name[50], struct Record *r);
void saveAccountToFile(FILE *ptr, struct User u, struct Record r);
void handleFailedRegistration(char a[69], char pass[69]);

// User functions
void registerUser();
void updateAccount(struct User u);
void checkAccountDetails(struct User u);
void makeTransaction(struct User u);
void removeAccount(struct User u);
void transferOwnership(struct User u);
char *getPasswordFromFile(struct User u, char *passwordBuffer, size_t bufferSize);

// New Function Declarations
void createNewAcc(struct User u);
void updateAccountInfo(struct User u);
void checkSingleAccount(struct User u);
void checkAllAccounts(struct User u);
void loginMenu(char a[69], char pass[69]);
void registerMenu(char a[69], char pass[69]);
void initMenu(struct User *u);

const char *getPassword(struct User u);
void mainMenu(struct User u);
void handleFailedLogin(struct User *u);

#endif
