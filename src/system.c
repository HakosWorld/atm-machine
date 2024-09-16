#include "header.h"

const char *RECORDS = "./data/records.txt";
const char *USERS = "./data/users.txt";

// Helper function to calculate interest based on account type
double calculateInterest(struct Record r) {
    if (strcmp(r.accountType, "saving") == 0) return r.amount * 0.07 / 12;
    if (strcmp(r.accountType, "fixed01") == 0) return r.amount * 0.04;
    if (strcmp(r.accountType, "fixed02") == 0) return r.amount * 0.05;
    if (strcmp(r.accountType, "fixed03") == 0) return r.amount * 0.08;
    return 0.0;
}

void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

// Helper function to get the account type interest description
void displayInterest(struct Record r) {
    if (strcmp(r.accountType, "saving") == 0)
        printf("You will get $%.2lf as interest on day %d of every month.\n", calculateInterest(r), r.deposit.day);
    else if (strcmp(r.accountType, "fixed01") == 0 || strcmp(r.accountType, "fixed02") == 0 || strcmp(r.accountType, "fixed03") == 0)
        printf("You will get $%.2lf as interest after the maturity period.\n", calculateInterest(r));
    else if (strcmp(r.accountType, "current") == 0)
        printf("You will not get interests because the account is of type current.\n");
}


int getUserId(const char *username) {
  FILE *fp = fopen("./data/users.txt", "r");
  if (!fp) {
    perror("\n\t\tError opening file");
    exit(1);
  }

  char id[20], name[20], pass[69];
  while (fscanf(fp, "%s %s %s", id, name, pass) != EOF) {
    if (strcmp(name, username) == 0) {
      fclose(fp);
      return atoi(id);
    }
  }
}

// Updated to include id and userId correctly
int getAccountFromFile(FILE *ptr, char name[50], struct Record *r) {
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
                  &r->userId,
                  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

// Updated to include id and userId correctly
void saveAccountToFile(FILE *ptr, struct User u, struct Record r) {
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
            r.id,
            u.id,
            u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u) {
  int option;
  do {
    if (notGood == 0) {
      printf("\n\t\t0: Try again! \n\t\t  1:main menu \n\t\t  2: exit! \n\n");
    } else {
      printf("\n\t\t1:main menu \n\t\t  0: exit! \n\n");
    }

    scanf("%d", &option);

    if (option == 0) {
      f(u);
      break;
    } else if (option == 1) {
      mainMenu(u);
      break;
    } else if (option == 2) {
      exit(0);
    } else {
      printf("Insert a valid operation!\n");
    }
  } while (option < 0 || option > 2);
}


void registerUser() {
    struct User newUser;
    char tempName[50];
    FILE *pf = fopen(USERS, "r+");
    if (pf == NULL) {
        printf("Error opening user file!\n");
        return;
    }

    printf("\t\t\t===== Register New User =====\n");
    printf("\nEnter a new username: ");
    scanf("%s", newUser.name);

    // Check if username already exists
    int usernameExists = 0;
    while (fscanf(pf, "%d %s %s", &newUser.id, tempName, newUser.password) != EOF) {
        if (strcmp(tempName, newUser.name) == 0) {
            usernameExists = 1;
            break;
        }
    }

    if (usernameExists) {
        printf("✖ Username already exists. Please choose another name.\n");
        fclose(pf);
        return;
    }

    // Reset file pointer to start for ID calculation
    fseek(pf, 0, SEEK_SET);

    // Assign new ID
    newUser.id = 1; // Initial ID
    int highestId = 0;
    while (fscanf(pf, "%d %s %s", &newUser.id, tempName, newUser.password) != EOF) {
        if (newUser.id > highestId) {
            highestId = newUser.id;
        }
    }
    newUser.id = highestId + 1;

    printf("Enter a password: ");
    scanf("%s", newUser.password);

    // Append new user to the file
    fprintf(pf, "%d %s %s\n", newUser.id, newUser.name, newUser.password);
    fclose(pf);
    printf("✔ Registration successful!\n");
}
// Update Account Information Feature
void updateAccount(struct User u) {
    struct Record r;
    struct Record updatedRecord;
    char userName[50];
    int accountId;
    FILE *pf = fopen(RECORDS, "r+");
    FILE *temp = fopen("temp.txt", "w");

    if (pf == NULL || temp == NULL) {
        printf("Error opening records file!\n");
        return;
    }

    printf("\t\t\t===== Update Account Information =====\n");
    printf("\nEnter the account ID to update: ");
    scanf("%d", &accountId);

    int found = 0;
    while (getAccountFromFile(pf, userName, &r)) {
        if (r.accountNbr == accountId && strcmp(userName, u.name) == 0) {
            found = 1;
            updatedRecord = r;

            int option;
            printf("What do you want to update?\n1. Phone number\n2. Country\n");
            scanf("%d", &option);

            switch (option) {
                case 1:
                    printf("Enter the new phone number: ");
                    scanf("%d", &updatedRecord.phone);
                    break;
                case 2:
                    printf("Enter the new country: ");
                    scanf("%s", updatedRecord.country);
                    break;
                default:
                    printf("Invalid option!\n");
                    fclose(pf);
                    fclose(temp);
                    remove("temp.txt");
                    return;
            }
        }
        saveAccountToFile(temp, u, found ? updatedRecord : r);
    }
    fclose(pf);
    fclose(temp);

    remove(RECORDS);
    rename("temp.txt", RECORDS);

    if (found)
        printf("✔ Account information updated successfully!\n");
    else
        printf("✖ Account not found.\n");
}

// Check details of a specific account feature
void checkAccountDetails(struct User u) {
    struct Record r;
    char userName[50]; // Adjusted size for username
    int accountId;
    FILE *pf = fopen(RECORDS, "r");

    if (pf == NULL) {
        printf("Error opening records file!\n");
        return;
    }

    printf("\t\t\t===== Check Account Details =====\n");
    printf("\nEnter the account ID to check: ");

    if (scanf("%d", &accountId) != 1) { // Error check for invalid input
        printf("\nInvalid input! Please enter a valid account ID.\n");
        fclose(pf);
        return;
    }

    int found = 0;
    while (getAccountFromFile(pf, userName, &r)) {
    if (r.accountNbr == accountId && strcmp(userName, u.name) == 0) {
      found = 1;
      break;
    }
  }
  fclose(pf);

  if (!found) {
    printf("\n\t\tNo account found with account number %d.\n", accountId);
    stayOrReturn(0, checkAccountDetails, u);
    return;
  }

  printf("\n\t\tAccount number: %d\n\t\tDeposit date: %d/%d/%d\n\t\tCountry: "
         "%s \n\t\tPhone number: %d \n\t\tAmount deposited: $%.2f \n\t\tType "
         "of account: %s\n",
         r.accountNbr, r.deposit.day, r.deposit.month, r.deposit.year,
         r.country, r.phone, r.amount, r.accountType);

  double rate;
  if (strcmp(r.accountType, "savings") == 0) {
    rate = 0.07;
    double interest = r.amount * (1 + rate / 12) - r.amount;
    printf("\n\t\tYou will get $%.2f as interest on day %d of every month.\n",
           interest, r.deposit.day);
  } else if (strcmp(r.accountType, "fixed01") == 0) {
    rate = 0.04;
    double interest = r.amount * (1 + rate / 12) - r.amount;
    interest *= 12;
    printf("\n\t\tYou will get $%.2f as interest on %d/%d/%d.\n", interest,
           r.deposit.month, r.deposit.day, r.deposit.year + 1);
  } else if (strcmp(r.accountType, "fixed02") == 0) {
    rate = 0.05;
    double interest = r.amount * (1 + rate / 12) - r.amount;
    interest *= 24;
    printf("\n\t\tYou will get $%.2f as interest on %d/%d/%d.\n", interest,
           r.deposit.month, r.deposit.day, r.deposit.year + 2);
  } else if (strcmp(r.accountType, "fixed03") == 0) {
    rate = 0.08;
    double interest = r.amount * (1 + rate / 12) - r.amount;
    interest *= 36;
    printf("\n\t\tYou will get $%.2f as interest on %d/%d/%d.\n", interest,
           r.deposit.month, r.deposit.day, r.deposit.year + 3);
  } else if (strcmp(r.accountType, "current") == 0) {
    printf("\n\t\tYou will not get interests because the account is of type "
           "current\n");
  } else {
    printf("\n\t\tInvalid account type.\n");
    stayOrReturn(0, checkAccountDetails, u);
    return;
  }
  success(u);
}


void makeTransaction(struct User u) {
    struct Record r;
    struct Record updatedRecord;
    char userName[50];
    int accountId;
    FILE *pf = fopen(RECORDS, "r+");
    FILE *temp = fopen("temp.txt", "w");

    if (pf == NULL || temp == NULL) {
        printf("Error opening records file!\n");
        return;
    }

    printf("\t\t\t===== Make Transaction =====\n");
    printf("\nEnter the account ID to transact: ");
    scanf("%d", &accountId);

    int found = 0;
    while (getAccountFromFile(pf, userName, &r)) {
        if (r.accountNbr == accountId && strcmp(userName, u.name) == 0) {
            found = 1;
            updatedRecord = r;

            // Prevent transactions on fixed accounts
            if (strcmp(r.accountType, "fixed01") == 0 || strcmp(r.accountType, "fixed02") == 0 || strcmp(r.accountType, "fixed03") == 0) {
                printf("✖ Transactions are not allowed on fixed accounts.\n");
                fclose(pf);
                fclose(temp);
                remove("temp.txt");  // Clean up the temp file
                return;
            }

            // Get user choice for deposit or withdrawal
            int option;
            double amount;
            printf("Enter 1 to deposit or 2 to withdraw: ");
            scanf("%d", &option);

            if (option == 1) {
                printf("Enter amount to deposit: $");
                scanf("%lf", &amount);
                updatedRecord.amount += amount;
                printf("✔ Successfully deposited $%.2f. New balance: $%.2f\n", amount, updatedRecord.amount);
            } else if (option == 2) {
                printf("Enter amount to withdraw: $");
                scanf("%lf", &amount);
                if (amount > updatedRecord.amount) {
                    printf("✖ Insufficient funds.\n");
                    fclose(pf);
                    fclose(temp);
                    remove("temp.txt");  
                    return;
                }
                updatedRecord.amount -= amount;
                printf("✔ Successfully withdrew $%.2f. New balance: $%.2f\n", amount, updatedRecord.amount);
            } else {
                printf("Invalid option!\n");
                fclose(pf);
                fclose(temp);
                remove("temp.txt");  
                return;
            }
        }

        // Write the updated record if the account was found, otherwise write the original record
        saveAccountToFile(temp, u, found ? updatedRecord : r);
    }

    fclose(pf);
    fclose(temp);

    if (found) {
        remove(RECORDS);               // Remove old records file
        rename("temp.txt", RECORDS);    // Replace with updated temp file
        printf("✔ Transaction completed successfully!\n");
        success(u);
            return;


    } else {
        remove("temp.txt");  // Clean up the temp file if no account was found
        printf("✖ Account not found.\n");
        stayOrReturn(0, makeTransaction, u);
        return;

    }
}

// Remove Existing Account Feature
void removeAccount(struct User u) {
    struct Record r;
    char userName[50];
    int accountId;
    FILE *pf = fopen(RECORDS, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (pf == NULL || temp == NULL) {
        printf("Error opening records file!\n");
        if (pf) fclose(pf);  // Close file if it was opened
        if (temp) fclose(temp);  // Close file if it was opened
        return;
    }

    printf("\t\t\t===== Remove Existing Account =====\n");
    printf("\nEnter the account ID to remove: ");
    scanf("%d", &accountId);

    int found = 0;
    while (getAccountFromFile(pf, userName, &r)) {
        if (r.accountNbr == accountId && strcmp(userName, u.name) == 0) {
            found = 1;
            printf("✔ Account removed successfully!\n");
            // Do not write this record to the temp file
        } else {
            saveAccountToFile(temp, u, r);  // Write non-matching records to the temp file
        }
    }

    fclose(pf);
    fclose(temp);

    if (found) {
        remove(RECORDS);               // Remove old records file
        rename("temp.txt", RECORDS);    // Replace with updated temp file
        success(u);
    } else {
        remove("temp.txt");  // Clean up the temp file if no account was found
        printf("✖ Account not found.\n");
        stayOrReturn(0, removeAccount, u);
    }
}

void transferOwnership(struct User u) {
    struct Record r;
    struct Record updatedRecord;
    struct User newOwner;
    char currentOwnerName[50];
    char newOwnerName[50];
    int accountId;
    FILE *pf = fopen(RECORDS, "r");
    FILE *uf = fopen(USERS, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (pf == NULL || uf == NULL || temp == NULL) {
        printf("Error opening files!\n");
        return;
    }

    printf("\t\t\t===== Transfer Ownership =====\n");
    printf("\nEnter the account ID to transfer: ");
    scanf("%d", &accountId);

    printf("Enter the new owner's username: ");
    scanf("%s", newOwnerName);

    // Search for the new owner in the user file
    int newOwnerFound = 0;
    while (fscanf(uf, "%d %s %s", &newOwner.id, newOwner.name, newOwner.password) != EOF) {
        if (strcmp(newOwner.name, newOwnerName) == 0) {
            newOwnerFound = 1;
            break;
        }
    }

    if (!newOwnerFound) {
        printf("✖ New owner not found.\n");
        fclose(pf);
        fclose(uf);
        fclose(temp);
        remove("temp.txt"); // Clean up the temp file
        return;
    }

    // Search for the account to transfer ownership
    int found = 0;
    while (getAccountFromFile(pf, currentOwnerName, &r)) {
        if (r.accountNbr == accountId && strcmp(currentOwnerName, u.name) == 0) {
            found = 1;
            updatedRecord = r;
            updatedRecord.userId = newOwner.id; // Transfer ownership to the new owner
            printf("✔ Account ownership will be transferred to %s.\n", newOwner.name);
            saveAccountToFile(temp, newOwner, updatedRecord); // Save the updated account
        } else {
            // For all other accounts, keep the original user and record
            saveAccountToFile(temp, u, r);
        }
    }

    fclose(pf);
    fclose(uf);
    fclose(temp);

    // Replace the old records file with the updated one
    remove(RECORDS);
    rename("temp.txt", RECORDS);

    if (found)
        printf("✔ Ownership transferred successfully!\n");
    else
        printf("✖ Account not found or transfer failed.\n");
}




void checkAllAccounts(struct User u) {
 
  FILE *pf = fopen(RECORDS, "r");
  if (pf == NULL) {
    perror("\n\t\tFailed to open file");
    return;
  }

  int accountsFound = 0;
  struct Record r;
  char userName[69];

  system("clear");
    printf("\t\t\t===== List of Owned Accounts =====\n");

  while (getAccountFromFile(pf, userName, &r)) {
    if (strcmp(userName, u.name) == 0) {
      accountsFound = 1;
      printf("\t\t_____________________\n");
         printf("\nAccount number: %d\nDeposit Date: %d/%d/%d \nCountry: %s \nPhone number: %d \nAmount deposited: $%.2f \nType Of Account: %s\n",
                   r.accountNbr, r.deposit.day, r.deposit.month, r.deposit.year,
                   r.country, r.phone, r.amount, r.accountType);
            displayInterest(r);
          //  printf("intert?");
    }
  }

  fclose(pf);

  if (accountsFound == 0) {
    printf("\t\tNo accounts found for %s.\n", u.name);
    stayOrReturn(1, checkAllAccounts, u);
    return;
  }

  success(u);
}

void checkSingleAccount(struct User u) {
    checkAccountDetails(u);
}

void updateAccountInfo(struct User u) {
    updateAccount(u);
}


void createNewAcc(struct User u) {
  struct Record r;
  struct Record cr;
  char userName[50];
  FILE *pf = fopen(RECORDS, "a+");
  FILE *rf = fopen(RECORDS, "r");

  if (!rf) {
    perror("\n\t\tError opening file");
    exit(1);
  }
int lastRecordId = 0;
while (fscanf(rf, "%d %*d %*s %d %d/%d/%d %s %d %lf %s\n",
              &r.id, &r.accountNbr, &r.deposit.month, &r.deposit.day, &r.deposit.year,
              r.country, &r.phone, &r.amount, r.accountType) != EOF) {
    if (r.id > lastRecordId) {
        lastRecordId = r.id;
    }
}

  fclose(rf);

  do {
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\n\t\tEnter today's date(mm/dd/yyyy): ");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);

    printf("\n\t\tEnter the account number: ");
    scanf("%d", &r.accountNbr);

    int accountExists = 0;

    rewind(pf);

    while (getAccountFromFile(pf, userName, &cr)) {
      if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr) {
        printf("\n\t\t✖ This Account already exists for this user\n");
        accountExists = 1;
        break;
      }
    }

    if (accountExists) {
      fclose(pf);
      stayOrReturn(0, createNewAcc, u);
    } else {
      r.id = lastRecordId + 1;
      u.id = getUserId(u.name);
      printf("\n\t\tEnter the country: ");
      scanf("%s", r.country);

      printf("\n\t\tEnter the phone number: ");
      scanf("%d", &r.phone);

      printf("\n\t\tEnter amount to deposit: $");
      scanf("%lf", &r.amount);

      printf("\n\t\tChoose the type of account:\n\n\t\t -> savings\n\t\t -> "
             "current\n\t\t -> fixed01(for 1 year)\n\t\t -> fixed02(for 2 "
             "years)\n\t\t -> "
             "fixed03(for 3 "
             "years)\n\n\t\tEnter your choice: ");
      scanf("%s", r.accountType);

      saveAccountToFile(pf, u, r);

      fclose(pf);
      printf("\n\t\t✔ Success!\n");
      success(u);
      break;
    }
  } while (1);
}



// Function to get the password of a user from file
char *getPasswordFromFile(struct User u, char *passwordBuffer, size_t bufferSize) {
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen(USERS, "r")) == NULL) {
        printf("Error! Opening file\n");
        exit(1);
    }

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF) {
        if (strcmp(userChecker.name, u.name) == 0) {
            fclose(fp);
            strncpy(passwordBuffer, userChecker.password, bufferSize - 1);
            passwordBuffer[bufferSize - 1] = '\0'; // Ensure null termination
            return passwordBuffer;
        }
    }

    fclose(fp);
    return NULL; // Return NULL if user is not found
}