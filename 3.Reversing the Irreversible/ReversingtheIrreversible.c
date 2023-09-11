#include <stdio.h>
#include <stdlib.h>

// Structure to represent a transaction node
struct Node {
    int transactionId;
    int transactionAmount;
    struct Node* next;
};

// Function to reverse the transactions
void reverseTransactions(struct Node** head, int* fraudulentTransactions, int numberOfFraudulentTransactions) {
    struct Node* current = *head;
    struct Node* previous = NULL;
    struct Node* next = NULL;

    // Traverse the linked list
    while (current != NULL) {
        // Check if the current transaction is fraudulent
        int isFraudulent = 0;
        int i;
        for (i = 0; i < numberOfFraudulentTransactions; i++) {
            if (current->transactionId == fraudulentTransactions[i]) {
                isFraudulent = 1;
                break;
            }
        }

        // If the transaction is fraudulent, delete it from the linked list
        if (isFraudulent) {
            // If the current node is the head, update the head
            if (current == *head) {
                *head = current->next;
                free(current);
                current = *head; // Move to the next node
            } else {
                // Update the previous node's next pointer
                previous->next = current->next;
                free(current);
                current = previous->next; // Move to the next node
            }
        } else {
            previous = current;
            current = current->next;
        }
    }
}

// Function to print the transactions
void printTransactions(struct Node* head) {
    struct Node* current = head;

    while (current != NULL) {
        printf("%d, %d\n", current->transactionId, current->transactionAmount);
        current = current->next;
    }
}

// Function to free the memory of the linked list
void freeLinkedList(struct Node* head) {
    struct Node* current = head;
    struct Node* next = NULL;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

int main(int argc, char* argv[]) {
    FILE* file = fopen("Test01.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    int numberOfTransactions, numberOfFraudulentTransactions;
    fscanf(file, "%d %d", &numberOfTransactions, &numberOfFraudulentTransactions);

    struct Node* head = NULL;
    struct Node* tail = NULL;

    // Read the transactions from the file and create the linked list
    int i;
    for (i = 0; i < numberOfTransactions; i++) {
        int transactionId, transactionAmount;
        fscanf(file, "%d,%d", &transactionId, &transactionAmount);

        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->transactionId = transactionId;
        newNode->transactionAmount = transactionAmount;
        newNode->next = NULL;

        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    // Read the fraudulent transactions from the file
    int* fraudulentTransactions = (int*)malloc(numberOfFraudulentTransactions * sizeof(int));
    int j;
    for (j = 0; j < numberOfFraudulentTransactions; j++) {
        fscanf(file, "%d", &fraudulentTransactions[j]);
    }

    fclose(file);

    // Reverse the fraudulent transactions
    reverseTransactions(&head, fraudulentTransactions, numberOfFraudulentTransactions);

    // Print the remaining transactions
    printTransactions(head);

    // Free the memory of the linked list
    freeLinkedList(head);
    free(fraudulentTransactions);

    return 0;
}


