/* A console-based food ordering system developed in C that allows users to register, log in, place food orders, and enables an admin to view all customer orders. The project uses file handling to store user information and order history permanently.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct User {
    char username[20];
    char password[20];
    char phone[15];
    char address[100];
};

struct Item {
    int id;
    char name[30];
    int price;
};

//MENU
struct Item menu[] = {
    {1, "Burger", 50},
    {2, "Pizza", 120},
    {3, "Pasta", 90},
    {4, "Sandwich", 60},
    {5, "Fries", 40},
    {6, "Coke", 30},
    {7, "Coffee", 70},
    {8, "IceCream", 80}
};

int menuSize = 8;

//Checking duplicate username
int userExists(char username[]) {
    FILE *fp = fopen("users.txt", "r");
    struct User u;

    if (fp == NULL)
        return 0;

    while (fscanf(fp, "%s %s %s", u.username, u.password, u.phone) != EOF) {
        fgets(u.address, sizeof(u.address), fp);

        if (strcmp(u.username, username) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

//Register
void registerUser() {
    FILE *fp = fopen("users.txt", "a");
    struct User u;

    printf("\n--- REGISTER ---\n");

    printf("Enter username: ");
    scanf("%s", u.username);

    if (userExists(u.username)) {
        printf("Username already exists!\n");
        fclose(fp);
        return;
    }

    printf("Enter phone: ");
    scanf("%s", u.phone);

    getchar(); 

    printf("Enter address: ");
    fgets(u.address, sizeof(u.address), stdin);

    u.address[strcspn(u.address, "\n")] = '\0';

    printf("Enter password: ");
    scanf("%s", u.password);

    fprintf(fp, "%s %s %s %s\n",
            u.username, u.password, u.phone, u.address);

    fclose(fp);

    printf("\nRegistration successful!\n");
}

//Login
int login(char loggedUser[]) {
    FILE *fp = fopen("users.txt", "r");
    struct User u;
    char username[20], password[20];

    if (fp == NULL) {
        printf("No users registered yet!\n");
        return 0;
    }

    printf("\n--- LOGIN ---\n");

    printf("Enter username: ");
    scanf("%s", username);

    printf("Enter password: ");
    scanf("%s", password);

    while (fscanf(fp, "%s %s %s", u.username, u.password, u.phone) != EOF) {
        fgets(u.address, sizeof(u.address), fp);

        if (strcmp(u.username, username) == 0 &&
            strcmp(u.password, password) == 0) {

            strcpy(loggedUser, username);
            fclose(fp);
            printf("\nLogin successful!\n");
            return 1;
        }
    }

    fclose(fp);
    printf("\nInvalid credentials!\n");
    return 0;
}

//Order View (Customers will be seeing the items in a menu form)
void placeOrder(char username[]) {
    int choice, qty;
    int total = 0;
    int day, month, year;

    FILE *fp = fopen("orders.txt", "a");

    printf("\nEnter Date (DD MM YYYY): ");
    scanf("%d %d %d", &day, &month, &year);

    printf("\n------ MENU ------\n");
    printf("(Press 0 to finish order)\n");

    for (int i = 0; i < menuSize; i++) {
        printf("%d. %-10s - Rs %d\n", menu[i].id, menu[i].name, menu[i].price);
    }

    printf("\n------ BILL ------\n");
    printf("User: %s\n", username);
    printf("Date: %02d-%02d-%d\n", day, month, year);

    fprintf(fp, "\nUser: %s Date: %02d-%02d-%d\n",
            username, day, month, year);

    printf("\nItem\t\tQty\tPrice\n");

    while (1) {
        printf("\nEnter item number: ");
        scanf("%d", &choice);

        if (choice == 0)
            break;

        if (choice < 1 || choice > menuSize) {
            printf("Invalid choice!\n");
            continue;
        }

        printf("Enter quantity: ");
        scanf("%d", &qty);

        int price = menu[choice - 1].price * qty;
        total += price;

        printf("%-10s\t%d\t%d\n", menu[choice - 1].name, qty, price);

        fprintf(fp, "%s %d %d\n",
                menu[choice - 1].name, qty, price);
    }

    printf("\nTotal = Rs %d\n", total);
    printf("-------------------------\n");

    fprintf(fp, "TOTAL: %d\n", total);

    fclose(fp);

    printf("Order saved successfully!\n");
}

// Admin View (Here all the orders will be shown)
void adminPanel() {
    char adminPass[20];
    FILE *fp;
    char ch;

    printf("\n--- ADMIN LOGIN ---\n");
    printf("Enter admin password: ");
    scanf("%s", adminPass);

    if (strcmp(adminPass, "admin123") != 0) { //Setting admin password as admin123
        printf("Wrong password!\n");
        return;
    }

    fp = fopen("orders.txt", "r");

    if (fp == NULL) {
        printf("No orders found!\n");
        return;
    }

    printf("\n====== ALL ORDERS ======\n");

    while ((ch = fgetc(fp)) != EOF) {
        printf("%c", ch);
    }

    printf("\n========================\n");

    fclose(fp);
}

int main() {
    int choice;
    char loggedUser[20];

    do {
        printf("\n===== MAIN MENU =====\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Admin Panel\n");
        printf("4. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerUser();
                break;

            case 2:
                if (login(loggedUser)) {
                    placeOrder(loggedUser);
                }
                break;

            case 3:
                adminPanel();
                break;

            case 4:
                printf("Thank you!\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while (choice != 4);

    return 0;
}