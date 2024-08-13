#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>

// File path for user data
#define USERS "./data/users.txt"

// Structure to hold user information
struct User {
    int id;
    char name[50];
    char password[50];
};

// Function to hide password input in Windows
void getPassword(char *pass) {
    char ch;
    int i = 0;
    while ((ch = _getch()) != '\r') {  // Read character until Enter is pressed
        if (ch == '\b') {  // Handle backspace
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            pass[i++] = ch;
            printf("*");  // Print asterisks instead of the actual characters
        }
    }
    pass[i] = '\0';
    printf("\n");
}

// Function to handle user login
void loginMenu(char a[50], char pass[50]) {
    system("cls");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%s", a);

    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    getPassword(pass);
}

// Function to get the password of a user from file
const char *getPassword(struct User u) {
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen(USERS, "r")) == NULL) {
        printf("Error! Opening file\n");
        exit(1);
    }

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF) {
        if (strcmp(userChecker.name, u.name) == 0) {
            fclose(fp);
            return userChecker.password;
        }
    }

    fclose(fp);
    return "no user found";
}

// Function to register a new user
void registerMenu(char a[50], char pass[50]) {
    FILE *fp;
    struct User newUser;
    struct User userChecker;
    int userId = 0;

    if ((fp = fopen(USERS, "a+")) == NULL) {
        printf("Error! Opening file\n");
        exit(1);
    }

    // Check for duplicate username
    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF) {
        if (strcmp(userChecker.name, a) == 0) {
            printf("✖ Username already exists. Please choose another username.\n");
            fclose(fp);
            return;
        }
        userId = userChecker.id + 1;
    }

    // Register the new user
    strcpy(newUser.name, a);
    strcpy(newUser.password, pass);
    newUser.id = userId;

    fprintf(fp, "%d %s %s\n", newUser.id, newUser.name, newUser.password);

    fclose(fp);
    printf("✔ Registration successful! You can now log in.\n");
}

// Main function
int main() {
    char username[50], password[50];
    int choice;

    while (1) {
        system("cls");
        printf("1. Login\n2. Register\n3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                loginMenu(username, password);
                // You can add login validation logic here
                break;
            case 2:
                printf("Enter username to register: ");
                scanf("%s", username);
                printf("Enter password to register: ");
                getPassword(password);
                registerMenu(username, password);
                break;
            case 3:
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
                break;
        }
        system("pause");  // Pause to view the result
    }

    return 0;
}
