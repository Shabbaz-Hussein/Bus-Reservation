#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define MAX_BUSES 50
#define MAX_SEATS 48
#define MAX_LOGIN_ATTEMPTS 3

// Struct for employee credentials
struct Employee {
    char username[25];
    char password[10];
};

// Struct for bus schedules
struct Bus {
    int busNumber;
    char source[50];
    char destination[50];
    char time[10];
    int availableSeats;
    int nextSeatNumber; // To track the next available seat number
};

// Struct for customer information
struct Customer {
    char name[50];
    char phoneNo[15];
    int seatNumber; // To store the assigned seat number
};

// Function to handle employee login
int employeeLogin(struct Employee employees[], int employeeCount, char username[], char password[]) {
    for (int i = 0; i < employeeCount; i++) {
        if (strcmp(employees[i].username, username) == 0 && strcmp(employees[i].password, password) == 0) {
            return i; // Return the index of the logged-in employee
        }
    }
    return -1; // Return -1 if login fails
}

// Function to add a new bus schedule
void addBusSchedule(struct Bus buses[], int *busCount) {
    // Collect bus schedule details from the employee
    printf("Enter Bus Number: ");
    scanf("%d", &buses[*busCount].busNumber);
    printf("Enter Source: ");
    scanf("%s", buses[*busCount].source);
    printf("Enter Destination: ");
    scanf("%s", buses[*busCount].destination);
    printf("Enter Time: ");
    scanf("%s", buses[*busCount].time);
    printf("Enter Available Seats: ");
    scanf("%d", &buses[*busCount].availableSeats);

    // Initialize the next available seat number for this bus
    buses[*busCount].nextSeatNumber = 1;

    (*busCount)++; // Increment the bus count
}

// Function to view all bus schedules
void viewBusSchedules(struct Bus buses[], int busCount) {
    system("cls"); // Clear the screen
    printf("\n_____ Bus Schedules _____\n");
    for (int i = 0; i < busCount; i++) {
        printf("Bus Number: %d\n", buses[i].busNumber);
        printf("Source: %s\n", buses[i].source);
        printf("Destination: %s\n", buses[i].destination);
        printf("Time: %s\n", buses[i].time);
        printf("Available Seats: %d\n", buses[i].availableSeats);
        printf("--------------------------\n");
    }
    printf("___________________________\n");
    printf("Press any key to go back...");
    getch(); // Wait for a key press
}

// Function to book seats for customers
void bookSeats(struct Bus buses[], int busCount, struct Customer customers[], int *customerCount) {
    printf("Enter Bus Number: ");
    int busNumber;
    scanf("%d", &busNumber);

    if (busNumber < 1 || busNumber > busCount) {
        printf("Invalid Bus Number. Please choose a valid Bus Number.\n");
        getch(); // Wait for a key press
        return;
    }

    if (buses[busNumber - 1].availableSeats == 0) {
        printf("No available seats on Bus %d. Please choose another bus.\n", busNumber);
        getch(); // Wait for a key press
        return;
    }

    printf("Enter Customer Name: ");
    fflush(stdin); // Clear the input buffer
    fgets(customers[*customerCount].name, sizeof(customers[*customerCount].name), stdin);
    customers[*customerCount].name[strcspn(customers[*customerCount].name, "\n")] = 0; // Remove the newline character

    printf("Enter Customer PhoneNo: ");
    scanf("%s", customers[*customerCount].phoneNo);

    // Assign the next available seat number and increment it
    customers[*customerCount].seatNumber = buses[busNumber - 1].nextSeatNumber;
    buses[busNumber - 1].nextSeatNumber++;

    // Display the receipt
    system("cls"); // Clear the screen
    printf("\n_______RECEIPT______\n");
    printf("NAME - %s\n", customers[*customerCount].name);
    printf("PHONENO - %s\n", customers[*customerCount].phoneNo);
    printf("BUS NUMBER - %d\n", buses[busNumber - 1].busNumber);
    printf("SOURCE - %s\n", buses[busNumber - 1].source);
    printf("DESTINATION - %s\n", buses[busNumber - 1].destination);
    printf("TIME - %s\n", buses[busNumber - 1].time);
    printf("SEAT NUMBER - %d\n", MAX_SEATS - buses[busNumber - 1].availableSeats + 1);
    printf("_________________________\n");
    printf("Press any key to continue...");
    getch(); // Wait for a key press

    // Save the receipt to Booking History
    FILE *bookingHistory = fopen("Booking History.txt", "a");
    if (bookingHistory == NULL) {
        printf("Error opening the Booking History file.\n");
        return;
    }
    fprintf(bookingHistory, "Customer Name: %s, PhoneNo: %s, Bus Number: %d, Source: %s, Destination: %s, Time: %s, Seat Number: %d\n",
        customers[*customerCount].name, customers[*customerCount].phoneNo, buses[busNumber - 1].busNumber,
        buses[busNumber - 1].source, buses[busNumber - 1].destination, buses[busNumber - 1].time,
        MAX_SEATS - buses[busNumber - 1].availableSeats + 1);
    fclose(bookingHistory);

    // Update available seats for the bus
    buses[busNumber - 1].availableSeats--;

    (*customerCount)++;
}

// Function to save bus schedules to a file
void saveBusSchedulesToFile(struct Bus buses[], int busCount) {
    FILE *file = fopen("Bus Schedules.txt", "w");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    for (int i = 0; i < busCount; i++) {
        fprintf(file, "%d,%s,%s,%s,%d,%d\n", buses[i].busNumber, buses[i].source, buses[i].destination, buses[i].time, buses[i].availableSeats, buses[i].nextSeatNumber);
    }

    fclose(file);
}

// Function to load bus schedules from a file
void loadBusSchedulesFromFile(struct Bus buses[], int *busCount) {
    FILE *file = fopen("Bus Schedules.txt", "r");
    if (file == NULL) {
        printf("No existing bus schedules found.\n");
        return;
    }

    *busCount = 0;
    while (fscanf(file, "%d,%49[^,],%49[^,],%9[^,],%d,%d\n", &buses[*busCount].busNumber, buses[*busCount].source, buses[*busCount].destination, buses[*busCount].time, &buses[*busCount].availableSeats, &buses[*busCount].nextSeatNumber) == 6) {
        (*busCount)++;
    }

    fclose(file);
}

// Function to view booking history
void viewBookingHistory() {
    system("cls"); // Clear the screen
    FILE *bookingHistory = fopen("Booking History.txt", "r");
    if (bookingHistory == NULL) {
        printf("No booking history found.\n");
        printf("Press any key to go back...");
        getch(); // Wait for a key press
        return;
    }

    printf("\n_____ Booking History _____\n");
    char line[100];
    while (fgets(line, sizeof(line), bookingHistory)) {
        printf("%s", line);
    }
    printf("___________________________\n");
    printf("Press any key to go back...");
    getch(); // Wait for a key press

    fclose(bookingHistory);
}

int main() {
    struct Employee employees[50]; // Assuming a maximum of 50 employees in the company
    struct Bus buses[50]; // Assuming a maximum of 50 bus schedules in the company
    struct Customer customers[50]; // Assuming a maximum of 50 customers
    int employeeCount = 0;
    int busCount = 0;
    int customerCount = 0;

    // Adding employee credentials
    strcpy(employees[employeeCount].username, "laki");
    strcpy(employees[employeeCount].password, "lak123");
    employeeCount++;

    // Adding employee credentials
    strcpy(employees[employeeCount].username, "saida");
    strcpy(employees[employeeCount].password, "sa1da");
    employeeCount++;

    int loggedInEmployeeIndex = -1; // To track the logged-in employee
    int loginAttempts = 0;

    int userType;

    while (1) {
        system("cls"); // Clear the screen
        printf("\n_____ Ahjin Bus Reservation _____\n");
        printf("1. Customer\n");
        printf("2. Employee Login\n");
        printf("3. Exit\n");
        printf("____________________________________\n");
        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            userType = 1; // Customer
            break;
        } else if (choice == 2) {
            userType = 2; // Employee
            break;
        } else if (choice == 3) {
            printf("Exiting the system. Thank You and Goodbye!\n");
            return 0;
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }

    if (userType == 2) {
        char username[25];
        char password[10];
        while (loggedInEmployeeIndex == -1 && loginAttempts < MAX_LOGIN_ATTEMPTS) {
            system("cls"); // Clear the screen
            printf("Enter username: ");
            scanf("%s", username);

            // Masking password
            printf("Enter password: ");
            int i = 0;
            while (1) {
                char c = getch();
                password[i] = c;
                if (c == 13) // Enter key
                    break;
                else
                    printf("*");
                i++;
            }
            password[i] = '\0';

            loggedInEmployeeIndex = employeeLogin(employees, employeeCount, username, password);

            if (loggedInEmployeeIndex == -1) {
                loginAttempts++;
                printf("Login failed. Please try again. You have %d attempts remaining.\n", MAX_LOGIN_ATTEMPTS - loginAttempts);
                printf("Press any key to continue...");
                getch(); // Wait for a key press
            } else {
                printf("Login successful!\n");
                printf("Press any key to continue...");
                getch(); // Wait for a key press
            }
        }

        if (loggedInEmployeeIndex == -1) {
            printf("Maximum login attempts reached. Exiting the system. Goodbye!\n");
            return 0;
        }
    }

    // Load existing bus schedules from a file
    loadBusSchedulesFromFile(buses, &busCount);

    while (1) {
        system("cls"); // Clear the screen
        printf("\n_____ Ahjin Bus Reservation _____\n");
        if (userType == 2) {
            printf("1. Add new bus schedule\n");
            printf("2. View all bus schedules\n");
            printf("3. View Booking History\n");
            printf("4. Logout\n");
        } else {
            printf("1. View all bus schedules\n");
            printf("2. Book a seat\n");
            printf("3. Exit\n");
        }
        printf("____________________________________\n");
        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (userType == 2) {
            if (choice == 1) {
                addBusSchedule(buses, &busCount);
                saveBusSchedulesToFile(buses, busCount); // Save the updated schedule to the file
                printf("Bus schedule added successfully!\n");
                printf("Press any key to continue...");
                getch(); // Wait for a key press
            } else if (choice == 2) {
                viewBusSchedules(buses, busCount);
            } else if (choice == 3) {
                if (loginAttempts >= MAX_LOGIN_ATTEMPTS) {
                    printf("You've reached the maximum login attempts. Exiting the system. Goodbye!\n");
                    printf("Press any key to continue...");
                    getch(); // Wait for a key press
                    break;
                }
                viewBookingHistory();
            } else if (choice == 4) {
                printf("Logging out. Thank You and Goodbye!\n");
                printf("Press any key to continue...");
                getch(); // Wait for a key press
                break;
            } else {
                printf("Invalid choice. Please try again.\n");
                printf("Press any key to continue...");
                getch(); // Wait for a key press
            }
        } else {
            if (choice == 1) {
                viewBusSchedules(buses, busCount);
            } else if (choice == 2) {
                // Customer booking functionality
                bookSeats(buses, busCount, customers, &customerCount);
                saveBusSchedulesToFile(buses, busCount); // Save the updated schedule to the file
            } else if (choice == 3) {
                printf("Exiting the system. Thank You and Goodbye!\n");
                printf("Press any key to continue...");
                getch(); // Wait for a key press
                break;
            } else {
                printf("Invalid choice. Please try again.\n");
                printf("Press any key to continue...");
                getch(); // Wait for a key press
            }
        }
    }

    return 0;
}
