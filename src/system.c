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

// Helper function to get the account type interest description
void displayInterest(struct Record r) {
    if (strcmp(r.accountType, "saving") == 0)
        printf("You will get $%.2lf as interest on day %d of every month.\n", calculateInterest(r), r.deposit.day);
    else if (strcmp(r.accountType, "fixed01") == 0 || strcmp(r.accountType, "fixed02") == 0 || strcmp(r.accountType, "fixed03") == 0)
        printf("You will get $%.2lf as interest after the maturity period.\n", calculateInterest(r));
    else if (strcmp(r.accountType, "current") == 0)
        printf("You will not get interests because the account is of type current.\n");
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

// Registration Feature
void registerUser() {
    struct User newUser;
    char tempName[50];
    FILE *pf = fopen(USERS, "a+");
    if (pf == NULL) {
        printf("Error opening user file!\n");
        return;
    }

    printf("\t\t\t===== Register New User =====\n");
    printf("\nEnter a new username: ");
    scanf("%s", newUser.name);

    while (fscanf(pf, "%d %s %s", &newUser.id, tempName, newUser.password) != EOF) {
        if (strcmp(tempName, newUser.name) == 0) {
            printf("✖ Username already exists. Please choose another name.\n");
            fclose(pf);
            return;
        }
    }

    newUser.id = newUser.id + 1; // increment ID based on the last one in the file
    printf("Enter a password: ");
    scanf("%s", newUser.password);

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
        if (r.id == accountId && strcmp(userName, u.name) == 0) {
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
    char userName[50];
    int accountId;
    FILE *pf = fopen(RECORDS, "r");

    if (pf == NULL) {
        printf("Error opening records file!\n");
        return;
    }

    printf("\t\t\t===== Check Account Details =====\n");
    printf("\nEnter the account ID to check: ");
    scanf("%d", &accountId);

    int found = 0;
    while (getAccountFromFile(pf, userName, &r)) {
        if (r.id == accountId && strcmp(userName, u.name) == 0) {
            found = 1;
            printf("\nAccount number: %d\nDeposit Date: %d/%d/%d \nCountry: %s \nPhone number: %d \nAmount deposited: $%.2f \nType Of Account: %s\n",
                   r.accountNbr, r.deposit.day, r.deposit.month, r.deposit.year,
                   r.country, r.phone, r.amount, r.accountType);
            displayInterest(r);
        }
    }
    fclose(pf);

    if (!found)
        printf("✖ Account not found.\n");
}

// Make Transaction Feature
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
        if (r.id == accountId && strcmp(userName, u.name) == 0) {
            found = 1;
            updatedRecord = r;

            if (strcmp(r.accountType, "fixed01") == 0 || strcmp(r.accountType, "fixed02") == 0 || strcmp(r.accountType, "fixed03") == 0) {
                printf("✖ Transactions are not allowed on fixed accounts.\n");
                fclose(pf);
                fclose(temp);
                remove("temp.txt");
                return;
            }

            int option;
            double amount;
            printf("Enter 1 to deposit or 2 to withdraw: ");
            scanf("%d", &option);

            if (option == 1) {
                printf("Enter amount to deposit: $");
                scanf("%lf", &amount);
                updatedRecord.amount += amount;
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
            } else {
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
        printf("✔ Transaction completed successfully!\n");
    else
        printf("✖ Account not found.\n");
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
        return;
    }

    printf("\t\t\t===== Remove Account =====\n");
    printf("\nEnter the account ID to remove: ");
    scanf("%d", &accountId);

    int found = 0;
    while (getAccountFromFile(pf, userName, &r)) {
        if (r.id != accountId || strcmp(userName, u.name) != 0) {
            saveAccountToFile(temp, u, r);
        } else {
            found = 1;
        }
    }
    fclose(pf);
    fclose(temp);

    remove(RECORDS);
    rename("temp.txt", RECORDS);

    if (found)
        printf("✔ Account removed successfully!\n");
    else
        printf("✖ Account not found.\n");
}

// Transfer Owner Feature
void transferOwnership(struct User u) {
    struct Record r;
    struct User newOwner;
    char userName[50];
    int accountId;
    FILE *pf = fopen(RECORDS, "r+");
    FILE *temp = fopen("temp.txt", "w");

    if (pf == NULL || temp == NULL) {
        printf("Error opening records file!\n");
        return;
    }

    printf("\t\t\t===== Transfer Ownership =====\n");
    printf("\nEnter the account ID to transfer: ");
    scanf("%d", &accountId);

    int found = 0;
    while (getAccountFromFile(pf, userName, &r)) {
        if (r.id == accountId && strcmp(userName, u.name) == 0) {
            found = 1;
            printf("Enter the new owner's username: ");
            scanf("%s", newOwner.name);

            FILE *uf = fopen(USERS, "r");
            int newOwnerFound = 0;
            while (fscanf(uf, "%d %s %s", &newOwner.id, newOwner.name, newOwner.password) != EOF) {
                if (strcmp(newOwner.name, newOwner.name) == 0) {
                    newOwnerFound = 1;
                    break;
                }
            }
            fclose(uf);

            if (newOwnerFound) {
                saveAccountToFile(temp, newOwner, r);
            } else {
                printf("✖ New owner not found.\n");
                fclose(pf);
                fclose(temp);
                remove("temp.txt");
                return;
            }
        } else {
            saveAccountToFile(temp, u, r);
        }
    }
    fclose(pf);
    fclose(temp);

    remove(RECORDS);
    rename("temp.txt", RECORDS);

    if (found)
        printf("✔ Ownership transferred successfully!\n");
    else
        printf("✖ Account not found.\n");
}

