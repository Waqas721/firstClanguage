#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent a node in the linked list
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Structure to represent the editor
typedef struct Editor {
    Node* head;
    Node* cursor;
    int undoCount;
    struct EditorState* undoStack[5]; // Stack to store previous editor states
} Editor;

// Structure to represent the editor state for undo functionality
typedef struct EditorState {
    Node* head;
    Node* cursor;
} EditorState;

Editor* createEditor() {
    Editor* editor = (Editor*)malloc(sizeof(Editor));
    editor->head = NULL;
    editor->cursor = NULL;
    editor->undoCount = 0;
    int i;
    for (i = 0; i < 5; i++) {
        editor->undoStack[i] = NULL;
    }
    return editor;
}

// Function to push the current editor state onto the undo stack
void pushEditorState(Editor* editor) {
    if (editor->undoCount >= 5) {
        // Remove the oldest state
        free(editor->undoStack[0]);
        // Shift the states to make room for the new state
        int i;
        for (i = 0; i < 4; i++) {
            editor->undoStack[i] = editor->undoStack[i + 1];
        }
        editor->undoCount = 4;
    }

    // Create a new editor state and copy the current state into it
    EditorState* newState = (EditorState*)malloc(sizeof(EditorState));
    newState->head = editor->head;
    newState->cursor = editor->cursor;

    // Push the new state onto the stack
    editor->undoStack[editor->undoCount++] = newState;
}

// Function to pop the previous editor state from the undo stack and restore the editor to that state
void popEditorState(Editor* editor) {
    if (editor->undoCount > 0) {
        // Get the previous state from the stack
        EditorState* prevState = editor->undoStack[--editor->undoCount];

        // Swap the current editor state with the previous state
        Node* tempHead = editor->head;
        Node* tempCursor = editor->cursor;
        editor->head = prevState->head;
        editor->cursor = prevState->cursor;
        prevState->head = tempHead;
        prevState->cursor = tempCursor;

        // Free the memory of the previous state
        free(prevState);
    }
}

void addNumber(Editor* editor, int number) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = number;
    newNode->next = NULL;

    if (editor->head == NULL) {
        editor->head = newNode;
        editor->cursor = newNode;
    } else {
        Node* current = editor->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void delete(Editor* editor) {
    if (editor->head == NULL) {
        return;
    }

    if (editor->head == editor->cursor) {
        editor->cursor = NULL;
    }

    Node* current = editor->head;
    Node* prev = NULL;
    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }

    if (prev != NULL) {
        prev->next = NULL;
    } else {
        editor->head = NULL;
    }

    free(current);
}

void addIndexNumber(Editor* editor, int index, int number) {
    if (index < 0) {
        return;
    }

    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = number;

    if (index == 0) {
        newNode->next = editor->head;
        editor->head = newNode;
        editor->cursor = newNode;
    } else {
        Node* current = editor->head;
        Node* prev = NULL;
        int i = 0;
        while (current != NULL && i < index) {
            prev = current;
            current = current->next;
            i++;
        }

        if (prev != NULL) {
            prev->next = newNode;
        } else {
            editor->head = newNode;
        }

        newNode->next = current;
        editor->cursor = newNode;
    }
}

void deleteIndex(Editor* editor, int index) {
    if (index < 0 || editor->head == NULL) {
        return;
    }

    if (index == 0) {
        Node* temp = editor->head;
        editor->head = editor->head->next;
        if (editor->cursor == temp) {
            editor->cursor = editor->head;
        }
        free(temp);
    } else {
        Node* current = editor->head;
        Node* prev = NULL;
        int i = 0;
        while (current != NULL && i < index) {
            prev = current;
            current = current->next;
            i++;
        }

        if (current != NULL) {
            prev->next = current->next;
            if (editor->cursor == current) {
                editor->cursor = prev->next;
            }
            free(current);
        }
    }
}

void moveForward(Editor* editor, int steps) {
    if (steps <= 0 || editor->cursor == NULL) {
        return;
    }

    int i = 0;
    while (i < steps && editor->cursor->next != NULL) {
        editor->cursor = editor->cursor->next;
        i++;
    }

    if (i > 0) {
        printf("Cursor moved forward by %d step(s).\n", i);
    }
}

Node* getNodeAtIndex(Editor* editor, int index) {
    if (index < 0 || editor->head == NULL) {
        return NULL;
    }

    Node* current = editor->head;
    int i = 0;
    while (current != NULL && i < index) {
        current = current->next;
        i++;
    }

    return current;
}

// Update the MoveBackward function
void moveBackward(Editor* editor, int steps) {
    if (steps <= 0 || editor->cursor == NULL || editor->cursor == editor->head) {
        return;
    }

    int i = 0;
    Node* current = editor->head;
    Node* prev = NULL;

    while (current != NULL && current->next != editor->cursor && i < steps) {
        prev = current;
        current = current->next;
        i++;
    }

    if (i < steps) {
        // If we didn't move the specified number of steps, move to the beginning
        while (current != NULL && current->next != NULL) {
            prev = current;
            current = current->next;
            i++;
        }
    }

    if (i < steps) {
        // If we still didn't move the specified number of steps, stop at the beginning
        editor->cursor = editor->head;
    } else {
        editor->cursor = prev;
    }

    printf("Cursor moved backward by %d step(s).\n", i);
}

void addImmediateNumber(Editor* editor, int number) {
    if (editor->cursor == NULL) {
        return;
    }

    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = number;
    newNode->next = editor->cursor->next;
    editor->cursor->next = newNode;
    editor->cursor = newNode;
}

void deleteImmediate(Editor* editor) {
    if (editor->cursor == NULL) {
        return;
    }

    if (editor->cursor == editor->head) {
        editor->head = editor->head->next;
        free(editor->cursor);
        editor->cursor = editor->head;
    } else {
        Node* current = editor->head;
        while (current != NULL && current->next != editor->cursor) {
            current = current->next;
        }

        if (current != NULL) {
            current->next = editor->cursor->next;
            free(editor->cursor);
            editor->cursor = current->next;
        }
    }
}

void print(Editor* editor) {
    Node* current = editor->head;
    printf("List: [");
    while (current != NULL) {
        if (current == editor->cursor) {
            printf("%d", current->data);
        } else {
            printf("%d", current->data);
        }
        current = current->next;
        if (current != NULL) {
            printf(", ");
        }
    }
    printf("]\n");
}


void updateIndexNumber(Editor* editor, int index, int number) {
    if (index < 0 || editor->head == NULL) {
        return;
    }

    Node* current = editor->head;
    int i = 0;
    while (current != NULL && i < index) {
        current = current->next;
        i++;
    }

    if (current != NULL) {
        current->data = number;
    }
}

void updateImmediateNumber(Editor* editor, int number) {
    if (editor->cursor != NULL) {
        editor->cursor->data = number;
    }
}

void shiftIndex(Editor* editor, int index) {
    if (index < 0 || editor->head == NULL || editor->cursor == NULL) {
        return;
    }

    Node* current = editor->head;
    int i = 0;
    while (current != NULL && i < index) {
        current = current->next;
        i++;
    }

    if (current != NULL) {
        int temp = editor->cursor->data;
        editor->cursor->data = current->data;
        current->data = temp;
    }
}

void undo(Editor* editor) {
    if (editor->undoCount > 0) {
        // Pop the previous state from the undo stack and restore the editor to that state
        EditorState* prevState = editor->undoStack[--editor->undoCount];
        editor->head = prevState->head;
        editor->cursor = prevState->cursor;

        // Free the memory of the previous state
        free(prevState);
    }
}

int main(int argc, char *argv[]){
    Editor* editor = createEditor();

    // Read input from file
    FILE* file = fopen("Test01.txt", "r");
    if (file == NULL) {
        printf("Failed to open file.\n");
        return 0;
    }

    char command[20];
    int arg1, arg2;

    while (fscanf(file, "%s", command) != EOF) {
        if (strcmp(command, "Add") == 0) {
            // Check if the next token is an integer
            if (fscanf(file, "%d", &arg1) == 1) {
                // Check if the next token is another integer
                if (fscanf(file, "%d", &arg2) == 1) {
                    // This is the "AddIndex" command
                    pushEditorState(editor);
                    addIndexNumber(editor, arg1, arg2);
                } else {
                    // This is the regular "Add" command
                    pushEditorState(editor);
                    addNumber(editor, arg1);
                }
            }
        } else if (strcmp(command, "Delete") == 0) {
            pushEditorState(editor);
            delete(editor);
        } else if (strcmp(command, "MoveForward") == 0) {
            fscanf(file, "%d", &arg1);
            pushEditorState(editor);
            moveForward(editor, arg1);
        } else if (strcmp(command, "MoveBackward") == 0) {
            fscanf(file, "%d", &arg1);
            pushEditorState(editor);
            moveBackward(editor, arg1);
        } else if (strcmp(command, "AddImmediate") == 0) {
            fscanf(file, "%d", &arg1);
            pushEditorState(editor);
            addImmediateNumber(editor, arg1);
        } else if (strcmp(command, "DeleteImmediate") == 0) {
            pushEditorState(editor);
            deleteImmediate(editor);
        } else if (strcmp(command, "Print") == 0) {
            print(editor);
        } else if (strcmp(command, "Update") == 0) {
            fscanf(file, "%d %d", &arg1, &arg2);
            pushEditorState(editor);
            updateIndexNumber(editor, arg1, arg2);
        } else if (strcmp(command, "UpdateImmediate") == 0) {
            fscanf(file, "%d", &arg1);
            pushEditorState(editor);
            updateImmediateNumber(editor, arg1);
        } else if (strcmp(command, "Shift") == 0) {
            fscanf(file, "%d", &arg1);
            pushEditorState(editor);
            shiftIndex(editor, arg1);
        } else if (strcmp(command, "Undo") == 0) {
           undo(editor);
        }
    }

    fclose(file);
    return 0;
} 
