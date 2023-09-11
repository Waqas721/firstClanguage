#include <stdio.h>
#include <stdlib.h>

// Structure for a node in the linked list
struct Node {
    int data;
    struct Node* next;
};

// Function to add a new node at the end of the linked list
void addNode(struct Node** head, int data) {
    // Create a new node
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;

    // If the linked list is empty, make the new node the head
    if (*head == NULL) {
        *head = newNode;
        return;
    }

    // Traverse to the end of the linked list
    struct Node* current = *head;
    while (current->next != NULL) {
        current = current->next;
    }

    // Add the new node at the end
    current->next = newNode;
}

// Function to delete the last node from the linked list
void deleteNode(struct Node** head) {
    // If the linked list is empty, return
    if (*head == NULL) {
        return;
    }

    // If there is only one node in the linked list, delete it
    if ((*head)->next == NULL) {
        free(*head);
        *head = NULL;
        return;
    }

    // Traverse to the second last node
    struct Node* current = *head;
    while (current->next->next != NULL) {
        current = current->next;
    }

    // Delete the last node
    free(current->next);
    current->next = NULL;
}

// Function to get the middle number(s) from the linked list
void getTrump(struct Node* head) {
    // If the linked list is empty, return
    if (head == NULL) {
        return;
    }

    // Count the number of nodes in the linked list
    int count = 0;
    struct Node* current = head;
    while (current != NULL) {
        count++;
        current = current->next;
    }

    // Find the middle number(s) based on the count
    int middle = count / 2;
    if (count % 2 == 0) {
        middle--;
    }

    // Traverse to the middle number(s) and print them
    current = head;
    int i;
    for (i = 0; i < middle; i++) {
        current = current->next;
    }

    if (count % 2 == 0) {
        printf("The middle two numbers are: %d and %d\n", current->data, current->next->data);
    } else {
        printf("The middle number is: %d\n", current->data);
    }
}

// Function to free the memory allocated for the linked list
void freeList(struct Node** head) {
    struct Node* current = *head;
    while (current != NULL) {
        struct Node* temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}

int main(int argc, char *argv[]) { 
    struct Node* head = NULL;
    FILE* file = fopen("Test05.txt", "r");
    char command[10];
    int number;

    // Read commands from the file
    while (fscanf(file, "%s", command) != EOF) {
        if (strcmp(command, "ADD") == 0) {
            fscanf(file, "%d", &number);
            addNode(&head, number);
        } else if (strcmp(command, "DELETE") == 0) {
            deleteNode(&head);
        } else if (strcmp(command, "END") == 0) {
            break;
        }
    }

    // Get the middle number(s) from the linked list
    getTrump(head);

    // Free the memory allocated for the linked list
    freeList(&head);

    // Close the file
    fclose(file);

    return 0;
}

