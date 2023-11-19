#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>

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
    float busFare;
    int availableSeats;
    char bookedSeats[MAX_SEATS + 1];
};

// Struct for customer information
struct Customer {
    char name[50];
    char phoneNo[15];
    char bookedSeat[4];
};

// Function to get the current time
void getCurrentTime(char currentTime[]) {
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Format the time as [Sun 12 Nov 2023 11:03am]
    strftime(currentTime, 30, "[%a %d %b %Y %I:%M%p]", timeinfo);
}

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
    printf("Enter Bus Fare (Ksh): ");
    scanf("%f", &buses[*busCount].busFare);
    
    // Initialize available seats
    buses[*busCount].availableSeats = MAX_SEATS;
    for (int i = 1; i <= MAX_SEATS; i++) {
        buses[*busCount].bookedSeats[i] = 0; // Initialize all seats as unbooked
    }

    (*busCount)++; // Increment the bus count
}

// Function to view all bus schedules
void viewBusSchedules(struct Bus buses[], int busCount) {
    system("cls");
    printf("\n_____ Bus Schedules _____\n");
    for (int i = 0; i < busCount; i++) {
        printf("Bus Number: %d\n", buses[i].busNumber);
        printf("Source: %s\n", buses[i].source);
        printf("Destination: %s\n", buses[i].destination);
        printf("Time: %s\n", buses[i].time);
        printf("Available Seats: %d\n", buses[i].availableSeats);
        printf("Bus Fare: Ksh %.2f\n", buses[i].busFare);
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
    fflush(stdin);
    fgets(customers[*customerCount].name, sizeof(customers[*customerCount].name), stdin);
    customers[*customerCount].name[strcspn(customers[*customerCount].name, "\n")] = 0;

    printf("Enter Customer PhoneNo: ");
    scanf("%s", customers[*customerCount].phoneNo);

    // Display available seats
    printf("Available Seats: ");
    for (int i = 1; i <= MAX_SEATS; i++) {
        if (!buses[busNumber - 1].bookedSeats[i]) {
            printf("S%02d ", i);
        }
    }
    printf("\n");

    // Allow the customer to choose seats
    printf("Choose Seats (e.g., S01,S02): ");
    char seatSelection[50];
    scanf("%s", seatSelection);

    // Parse seat selections
    char *token = strtok(seatSelection, ",");
    while (token != NULL) {
        int seatIndex = atoi(token + 1);
        if (seatIndex < 1 || seatIndex > MAX_SEATS || buses[busNumber - 1].bookedSeats[seatIndex]) {
            printf("Invalid seat selection. Please choose an available seat.\n");
            getch(); // Wait for a key press
            return;
        }

        // Update seat status
        buses[busNumber - 1].bookedSeats[seatIndex] = 1;

        // Display the receipt for each seat
        system("cls");
        char bookingTime[30];
		getCurrentTime(bookingTime);
		printf("Booking Time: %s\n", bookingTime);
        printf("\n_______RECEIPT______\n");
        printf("NAME - %s\n", customers[*customerCount].name);
        printf("PHONENO - %s\n", customers[*customerCount].phoneNo);
        printf("BUS NUMBER - %d\n", buses[busNumber - 1].busNumber);
        printf("SOURCE - %s\n", buses[busNumber - 1].source);
        printf("DESTINATION - %s\n", buses[busNumber - 1].destination);
        printf("TIME - %s\n", buses[busNumber - 1].time);
        printf("SEAT - %s\n", token);
        printf("BUS FARE - Ksh%.2f\n", buses[busNumber - 1].busFare);
        printf("_________________________\n");
        printf("Press any key to continue...");
        getch(); // Wait for a key press

        // Save the receipt to Booking History
        FILE *bookingHistory = fopen("Booking History.txt", "a");
        if (bookingHistory == NULL) {
            printf("Error opening the Booking History file.\n");
            return;
        }

		fprintf(bookingHistory, "Booking Time: %s\n", bookingTime);
        fprintf(bookingHistory, "Customer Name: %s\n", customers[*customerCount].name);
        fprintf(bookingHistory, "PhoneNo: %s\n", customers[*customerCount].phoneNo);
        fprintf(bookingHistory, "Bus Number: %d\n", buses[busNumber - 1].busNumber);
        fprintf(bookingHistory, "Source: %s\n", buses[busNumber - 1].source);
        fprintf(bookingHistory, "Destination: %s\n", buses[busNumber - 1].destination);
        fprintf(bookingHistory, "Time: %s\n", buses[busNumber - 1].time);
        fprintf(bookingHistory, "Seat: %s\n", token);
        fprintf(bookingHistory, "Bus Fare: Ksh%.2f\n", buses[busNumber - 1].busFare);
        fprintf(bookingHistory, "---------------------------------\n");

        // Close the file
        fclose(bookingHistory);

        // Update available seats for the bus
        buses[busNumber - 1].availableSeats--;

        // Move to the next seat
        token = strtok(NULL, ",");
    }

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
        fprintf(file, "%d,%s,%s,%s,%.2f,%d,", buses[i].busNumber, buses[i].source, buses[i].destination, buses[i].time, buses[i].busFare, buses[i].availableSeats);
        for (int j = 1; j <= MAX_SEATS; j++) {
            fprintf(file, "%d", buses[i].bookedSeats[j]);
            if (j < MAX_SEATS) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
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
    while (fscanf(file, "%d,%49[^,],%49[^,],%9[^,],%f,%d,", &buses[*busCount].busNumber, buses[*busCount].source, buses[*busCount].destination, buses[*busCount].time, &buses[*busCount].busFare, &buses[*busCount].availableSeats) == 6) {
        for (int i = 1; i <= MAX_SEATS; i++) {
            fscanf(file, "%d,", &buses[*busCount].bookedSeats[i]);
        }
        (*busCount)++;
    }

    fclose(file);
}

// Function to view booking history
void viewBookingHistory() {
    system("cls");
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
    struct Employee employees[50];
    struct Bus buses[50];
    struct Customer customers[50];
    int employeeCount = 0;
    int busCount = 0;
    int customerCount = 0;

    // Adding employee credentials
    strcpy(employees[employeeCount].username, "laki");
    strcpy(employees[employeeCount].password, "lak123");
    employeeCount++;

    // Adding employee credentials
    strcpy(employees[employeeCount].username, "shabz");
    strcpy(employees[employeeCount].password, "sh@bz123");
    employeeCount++;

    int loggedInEmployeeIndex = -1; // To track the logged-in employee
    int loginAttempts = 0;

    int userType;

    while (1) {
        system("cls");
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
            system("cls");
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
        system("cls");
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
            printf("In case you want to cancel booking, please contact customer care services\n");
        	printf("			Phone: 0740128562 / 0750035082\n");
        }
        printf("____________________________________\n");
        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (userType == 2) {
            if (choice == 1) {
                addBusSchedule(buses, &busCount);
                saveBusSchedulesToFile(buses, busCount);
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
                bookSeats(buses, busCount, customers, &customerCount);
                saveBusSchedulesToFile(buses, busCount);
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
