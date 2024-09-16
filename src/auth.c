#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include "header.h"

// File path for user data
#define USERS "./data/users.txt"


// // Function to hide password input in Linux
// void getPassword(char *pass) {
//     struct termios oldt, newt;
//     int i = 0;
//     char ch;

//     // Turn off terminal echo
//     tcgetattr(STDIN_FILENO, &oldt);
//     newt = oldt;
//     newt.c_lflag &= ~(ECHO);
//     tcsetattr(STDIN_FILENO, TCSANOW, &newt);

//     // Get password input
//     while ((ch = getchar()) != '\n') {
//         if (ch == '\b' && i > 0) {  // Handle backspace
//             i--;
//             printf("\b \b");
//         } else {
//             pass[i++] = ch;
//             printf("*");  // Print asterisks instead of actual characters
//         }
//     }
//     pass[i] = '\0';
//     printf("\n");

//     // Restore terminal echo
//     tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
// }

const char *getPassword(struct User u) {
  FILE *fp;
  struct User userChecker;
  char id[69];

  if ((fp = fopen("./data/users.txt", "r")) == NULL) {
    printf("Error! opening file");
    exit(1);
  }

  while (fscanf(fp, "%s %s %s", id, userChecker.name, userChecker.password) !=
         EOF) {
    if (strcmp(userChecker.name, u.name) == 0) {
      fclose(fp);
      u.id = atoi(id);
      char *buff = userChecker.password;
      return buff;
    }
  }

  fclose(fp);
  return "no user found";
}




// Function to handle user login
void loginMenu(char a[69], char pass[69]) {
  struct termios oflags, nflags;

  system("clear");
  printf("\n\n\t\tBank Management System\n\n\t\tUser Login: ");
  scanf("%s", a);

  // disabling echo
  tcgetattr(fileno(stdin), &oflags);
  nflags = oflags;
  nflags.c_lflag &= ~ECHO;
  nflags.c_lflag |= ECHONL;

  if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0) {
    perror("tcsetattr");
    return exit(1);
  }
  printf("\n\t\tEnter the password: ");
  scanf("%s", pass);

  // restore terminal
  if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) {
    perror("tcsetattr");
    return exit(1);
  }
};


// // Function to register a new user
// void registerMenu(char *username, char *password) {
//     FILE *fp;
//     struct User newUser;
//     struct User userChecker;
//     int userId = 0;
//     int maxId = -1;

//     if ((fp = fopen(USERS, "r+")) == NULL) {
//         printf("Error! Opening file\n");
//         exit(1);
//     }

//     // Check for duplicate username and find the highest ID
//     while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF) {
//         if (strcmp(userChecker.name, username) == 0) {
//             printf("✖ Username already exists. Please choose another username.\n");
//             fclose(fp);
//             return;
//         }
//         if (userChecker.id > maxId) {
//             maxId = userChecker.id;
//         }
//     }

//     // Register the new user
//     strcpy(newUser.name, username);
//     strcpy(newUser.password, password);
//     newUser.id = maxId + 1; // Increment the highest ID found

//     fseek(fp, 0, SEEK_END); // Move to the end of the file
//     fprintf(fp, "%d %s %s\n", newUser.id, newUser.name, newUser.password);

//     fclose(fp);
//     printf("✔ Registration successful! You can now log in.\n");
// }
int isUsernameUnique(char username[]) {
  FILE *fp;
  struct User userChecker;

  if ((fp = fopen("./data/users.txt", "r")) == NULL) {
    printf("Error! opening file");
    exit(1);
  }

  while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name,
                userChecker.password) != EOF) {
    if (strcmp(userChecker.name, username) == 0) {
      fclose(fp);
      return 0;
    }
  }
  fclose(fp);
  return 1;
}

void registerMenu(char a[69], char pass[69]) {
  struct termios oflags, nflags;

  do {
    system("clear");
    printf("\n\n\t\tBank Management System\n\n\t\t  User Registration\n");
    printf("\n\n\t\tEnter Username: ");
    scanf("%s", a);

    if (!isUsernameUnique(a)) {
      system("clear");
      printf("\n\n\t\tBank Management System\n\n\t\t  User Registration\n");
      printf("\n\t\tUsername already exists. \n");
      handleFailedRegistration(a, pass);
    }
  } while (!isUsernameUnique(a));

  // disabling echo
  tcgetattr(fileno(stdin), &oflags);
  nflags = oflags;
  nflags.c_lflag &= ~ECHO;
  nflags.c_lflag |= ECHONL;

  if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0) {
    perror("tcsetattr");
    exit(1);
  }
  printf("\n\t\tEnter Password: ");
  scanf("%s", pass);

  // restore terminal
  if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) {
    perror("tcsetattr");
    exit(1);
  }

  FILE *fp;
  struct User userChecker;
  char id[69];
  int lastUserID = 0;

  if ((fp = fopen("./data/users.txt", "r")) == NULL) {
    printf("Error! opening file");
    exit(1);
  }

  while (fscanf(fp, "%s %s %s", id, userChecker.name, userChecker.password) !=
         EOF) {
    lastUserID = atoi(id);
  }

  fclose(fp);

  if ((fp = fopen("./data/users.txt", "a")) == NULL) {
    printf("Error! opening file");
    exit(1);
  }

  lastUserID++;

  fprintf(fp, "%d %s %s\n", lastUserID, a, pass);

  fclose(fp);
}


void handleFailedRegistration(char a[69],
                              char pass[69]) {
  int option;
  do {
    printf("\n\t\tEnter 0 to try again, 1 to exit!\n\n");
    scanf("%d", &option);
    if (option == 0) {
      registerMenu(a, pass);
      break;
    } else if (option == 1) {
      exit(1);
      break;
    } else {
      printf("\t\tInsert a valid operation!\n");
    }
  } while (option < 0 || option > 1);
}

void handleFailedLogin(struct User *u) {
  int option;
  do {
    printf("\n\t\tEnter 0 to try again, 1 to exit! \n\n");
    scanf("%d", &option);

    if (option == 0) {
      initMenu(u);
      break;
    } else if (option == 1) {
      exit(1);
      break;
    } else {
      printf("Insert a valid operation!\n");
    }
  } while (option < 0 || option > 1);
}

