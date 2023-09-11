#include <stdio.h>
#include <stdlib.h>

// Structure for a node in the linked list
struct Node {
    int data;
    struct Node* next;
};

// Function to find the third last element of a linked list
int findThirdLast(struct Node* head) {
    // Check if the linked list has less than three elements
    if (head == NULL || head->next == NULL || head->next->next == NULL) {
        return -1;
    }

    struct Node* current = head;
    while (current->next->next->next != NULL) {
        current = current->next;
    }

    return current->data;
}

// Function to create a new node in the linked list
struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Function to insert a node at the end of the linked list
void insertNode(struct Node** head, int data) {
    struct Node* newNode = createNode(data);

    if (*head == NULL) {
        *head = newNode;
    } else {
        struct Node* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Function to print the linked list
void printList(struct Node* head) {
    struct Node* current = head;
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    struct Node* head = NULL;

    // Read the input from the file
    FILE* file = fopen("Test05.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return 0;
    }

    int num;
    while (fscanf(file, "%d", &num) != EOF) {
        insertNode(&head, num);
    }

    fclose(file);

    printf("Linked List: ");
    printList(head);

    int thirdLast = findThirdLast(head);
    if (thirdLast == -1) {
        printf("The linked list has less than three elements\nThird Last Element:-1 .\n");
    } else {
        printf("Third Last Element: %d\n", thirdLast);
    }

    return 0;
}

