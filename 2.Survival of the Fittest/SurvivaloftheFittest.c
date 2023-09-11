#include <stdio.h>
#include <stdlib.h>

// Structure for a node in the linked list
struct Node {
    int data;
    struct Node* next;
};

// Function to insert a new node at the beginning of the linked list
void insert(struct Node** head, int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = *head;
    *head = newNode;
}

// Function to sort the linked list in ascending order using insertion sort
void sortLinkedList(struct Node** head) {
    struct Node* sortedList = NULL;
    struct Node* current = *head;

    while (current != NULL) {
        struct Node* nextNode = current->next;

        // Insert current node into sorted list
        if (sortedList == NULL || current->data <= sortedList->data) {
            current->next = sortedList;
            sortedList = current;
        } else {
            struct Node* temp = sortedList;

            while (temp->next != NULL && temp->next->data < current->data) {
                temp = temp->next;
            }

            current->next = temp->next;
            temp->next = current;
        }

        current = nextNode;
    }

    *head = sortedList; // Update head pointer to point to the sorted list
}

// Function to calculate the median of the linked list
float calculateMedian(struct Node* head) {
    int count = 0, middleValue, prevValue, currentValue;

    // Counting the number of nodes in the linked list
    struct Node* tempNode = head;
    while (tempNode != NULL) {
        count++;
        tempNode = tempNode->next;
    }

    // Finding the middle value(s)
    middleValue = count / 2;
    prevValue = -1;
    currentValue = 0;
    tempNode = head;

    while (currentValue < middleValue) {
        prevValue = tempNode->data;
        tempNode = tempNode->next;
        currentValue++;
    }

    // If the number of nodes is odd, return the middle value
    if (count % 2 != 0) {
        return tempNode->data;
    } else {
        // If the number of nodes is even, return the average of the two middle values
        return (prevValue + tempNode->data) / 2.0;
    }
}

// Function to print the linked list
void printList(struct Node* head) {
    struct Node* temp = head;
    while (temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    struct Node* head = NULL;

    // Read numbers from a file and insert them into the linked list
    FILE* file = fopen("Test03.txt", "r");

    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    int num;

    while (fscanf(file, "%d", &num) != EOF) {
        insert(&head, num);

        // Sort the linked list after each insertion using insertion sort
        sortLinkedList(&head);

        // Calculate and print the median
        float median = calculateMedian(head);
        printf("Median: %.1f\n", median);
    }

    fclose(file);

    return 0;
}


