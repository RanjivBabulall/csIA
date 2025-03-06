#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_USERS 20

typedef struct {
    char username[30];
    char password[20];

    float weight;
    float height;
    float bmi;
} User;

void saveUsers(User users[MAX_USERS], int userCount) {
    FILE *fp = fopen("usersData.txt", "w");
    if (fp == NULL) {
        printf("Error opening file.");
        return;
    }
    for (int i = 0; i < userCount; i++) {
        fprintf(fp, "%s %s %.2f %.2f %.2f\n", users[i].username, users[i].password, users[i].weight, users[i].height, users[i].bmi);
    }
    fclose(fp);
}

int loadUsers(User users[MAX_USERS]) {
    FILE *fp = fopen("usersData.txt", "r");
    if (fp == NULL) return 0;

    int count = 0;
    while (count < MAX_USERS && fscanf(fp, "%s %s %f %f %f", users[count].username, users[count].password, &users[count].weight, &users[count].height, &users[count].bmi) == 5) {
        count++;
    }
    fclose(fp);
    return count;
}

void loginSignUp(User users[MAX_USERS], int *userCount, int *userIndex) {
    *userCount = loadUsers(users);
    int ans;
    while (1) {
        printf("\n************* WELCOME *************\n");
        printf("Login (1)\n");
        printf("Sign Up (2)\n");
        scanf("%d", &ans);
        getchar();

        switch (ans) {
            case 1: {
                char username[30];
                char password[20];
                printf("Enter name: ");
                fgets(username, 30, stdin);
                username[strcspn(username, "\n")] = 0;
                
                printf("Enter password: ");
                fgets(password, 20, stdin);
                password[strcspn(password, "\n")] = 0;
                
                for (int i = 0; i < *userCount; i++) {
                    if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
                        printf("Login successful. Welcome %s.\n", username);
                        *userIndex = i;
                        return;
                    }
                }
                printf("Invalid username or password\n");
                break;
            }
            case 2: {
                if (*userCount >= MAX_USERS) {
                    printf("Maximum users.\n");
                    continue;
                }
                
                User newUser;
                printf("Enter name: ");
                fgets(newUser.username, 30, stdin);
                newUser.username[strcspn(newUser.username, "\n")] = 0;
                
                printf("Enter password: ");
                fgets(newUser.password, 20, stdin);
                newUser.password[strcspn(newUser.password, "\n")] = 0;
                
                for (int i = 0; i < *userCount; i++) {
                    if (strcmp(users[i].username, newUser.username) == 0) {
                        printf("Username already exists\n");
                        return;
                    }
                }
                
                printf("Enter weight (kg): ");
                scanf("%f", &newUser.weight);
                getchar();
                
                printf("Enter height (m): ");
                scanf("%f", &newUser.height);
                getchar();
                
                newUser.bmi = newUser.weight / (newUser.height * newUser.height);
                printf("Your BMI is %.2f\n", newUser.bmi);
                
                users[(*userCount)++] = newUser;
                saveUsers(users, *userCount);
                printf("Registration successful. Welcome %s.\n", newUser.username);
                break;
            }
            default:
                printf("Invalid choice\n");
        }
    }
}

void update(User *user) {
    int ans1;
    printf("What would you like to change? \n");
    printf("Weight (1) \n");
    printf("Height (2) \n");
    scanf("%d", &ans1);
    getchar();
    
    switch (ans1) {
        case 1:
            printf("Enter new weight: ");
            scanf("%f", &user->weight);
            user->bmi = user->weight / (user->height * user->height);
            printf("\nBMI Updated: %f", user->bmi);
            break;
        case 2:
            printf("Enter new height: ");
            scanf("%f", &user->height);
            user->bmi = user->weight / (user->height * user->height);
            printf("\nBMI Updated: %.2f", user->bmi);
            break;
        default:
            printf("Invalid choice.\n");
            return;
    }
}

void menu(User users[MAX_USERS], int *userCount, int userIndex){
    int ans2;

    while(1){
        printf("\n******** MENU ********\n");
        printf("Update Your Information (1)\n");
        printf("Log out (2)\n");
        scanf("%d", &ans2);

        if (ans2 == 1) {
            update(&users[userIndex]);
            saveUsers(users, *userCount);
        } else if (ans2 == 2) {
            printf("Logging out.\n");
            break;
        } else {
            printf("Invalid choice\n");
        }
    }
}

int main(){

    User users[20];
    int userCount = 0;
    int userIndex = -1;

    loginSignUp(users, &userCount, &userIndex);

    if(userIndex != -1){
        menu(users, &userCount, userIndex);
    }
    return 0;
}
