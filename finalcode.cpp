#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4

// Variables
int available[NUMBER_OF_RESOURCES]; 
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]; 
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]; 
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES]; 

// Function to check if the system is in a safe state
bool isSafeState() {
    vector<bool> finish(NUMBER_OF_CUSTOMERS, false);
    int work[NUMBER_OF_RESOURCES];
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        work[i] = available[i];
    }

    while (true) {
        bool found = false;
        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
            if (!finish[i]) {
                bool canAllocate = true;
                for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                    if (need[i][j] > work[j]) {
                        canAllocate = false;
                        break;
                    }
                }

                if (canAllocate) {
                    for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                        work[j] += allocation[i][j];
                    }
                    finish[i] = true;
                    found = true;
                }
            }
        }
        if (!found) break;
    }

    for (bool f : finish) {
        if (!f) return false;
    }
    return true;
}

// Request resources
bool request_resources(int customer_num, int request[]) {
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (request[i] > need[customer_num][i] || request[i] > available[i]) {
            cout << "Request cannot be granted.\n";
            return false;
        }
    }

    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] -= request[i];
        allocation[customer_num][i] += request[i];
        need[customer_num][i] -= request[i];
    }

    if (isSafeState()) {
        cout << "Request granted.\n";
        return true;
    } else {
        cout << "Request denied to avoid unsafe state.\n";
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
            available[i] += request[i];
            allocation[customer_num][i] -= request[i];
            need[customer_num][i] += request[i];
        }
        return false;
    }
}

// Release resources
void release_resources(int customer_num, int release[]) {
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        allocation[customer_num][i] -= release[i];
        available[i] += release[i];
        need[customer_num][i] += release[i];
    }
    cout << "Resources released.\n";
}

// Display current state
void display_state() {
    cout << "\nAvailable Resources:\n";
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        cout << available[i] << " ";
    }
    cout << "\n\nMaximum Resources:\n";
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            cout << maximum[i][j] << " ";
        }
        cout << endl;
    }
    cout << "\nAllocated Resources:\n";
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            cout << allocation[i][j] << " ";
        }
        cout << endl;
    }
    cout << "\nNeeded Resources:\n";
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            cout << need[i][j] << " ";
        }
        cout << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != NUMBER_OF_RESOURCES + 1) {
        cout << "Usage: ./a.out <resource1> <resource2> ... <resource" << NUMBER_OF_RESOURCES << ">\n";
        return 1;
    }

    // Read available resources from command-line arguments
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] = stoi(argv[i + 1]);
    }

    // Read maximum resources from file
    ifstream maxFile("maximum.txt");
    if (!maxFile.is_open()) {
        cout << "Error opening file.\n";
        return 1;
    }

    string line;
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        getline(maxFile, line);
        stringstream ss(line);
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            ss >> maximum[i][j];
            need[i][j] = maximum[i][j]; // Initially, need is the same as maximum
        }
    }
    maxFile.close();

    // Read allocation matrix from user input
    cout << "Enter allocation matrix:\n";
for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
    for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
        cin >> allocation[i][j];
        need[i][j] -= allocation[i][j]; // Update need matrix
    }
}

// Now update available resources after reading the entire allocation matrix
for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
    for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
        available[j] -= allocation[i][j];
    }
}

    while (true) {
        cout << "\nOptions:\n1. Request Resources\n2. Release Resources\n3. Display State\n4. Exit\nEnter choice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            cout << "Enter customer number and resource request:\n";
            int customer_num, request[NUMBER_OF_RESOURCES];
            cin >> customer_num;
            for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
                cin >> request[i];
            }
            request_resources(customer_num, request);
        } else if (choice == 2) {
            cout << "Enter customer number and resources to release:\n";
            int customer_num, release[NUMBER_OF_RESOURCES];
            cin >> customer_num;
            for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
                cin >> release[i];
            }
            release_resources(customer_num, release);
        } else if (choice == 3) {
            display_state();
        } else if (choice == 4) {
            break;
        } else {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}