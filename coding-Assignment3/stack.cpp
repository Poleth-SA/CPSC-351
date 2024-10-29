#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Linked list node
typedef int value_t;
typedef struct Node {
    value_t data;
    struct Node *next;
} StackNode;

// Stack function declarations
void push(value_t v, StackNode **top);
value_t pop(StackNode **top);
int is_empty(StackNode *top);

// Mutex for thread safety
pthread_mutex_t stack_mutex = PTHREAD_MUTEX_INITIALIZER;

int main(void) {
    StackNode *top = NULL;

    // Create 200 concurrent threads
    pthread_t threads[200];
    for (int i = 0; i < 200; i++) {
        pthread_create(&threads[i], NULL, testStack, &top);
    }

    // Wait for all threads to complete
    for (int i = 0; i < 200; i++) {
        pthread_join(threads[i], NULL);
    }

    // Clean up the stack
    while (!is_empty(top)) {
        pop(&top);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&stack_mutex);
    return 0;
}

// Thread function that intermixes push and pop operations
void *testStack(void *arg) {
    StackNode **top = (StackNode **)arg;
    for (int i = 0; i < 500; i++) {
        push(rand() % 100, top);
        push(rand() % 100, top);
        push(rand() % 100, top);
        pop(top);
        pop(top);
        pop(top);
    }
    return NULL;
}

void push(value_t v, StackNode **top) {
    pthread_mutex_lock(&stack_mutex);
    StackNode *new_node = (StackNode*)malloc(sizeof(StackNode));
    new_node->data = v;
    new_node->next = *top;
    *top = new_node;
    pthread_mutex_unlock(&stack_mutex);
}

value_t pop(StackNode **top) {
    pthread_mutex_lock(&stack_mutex);
    if (is_empty(*top)) {
        pthread_mutex_unlock(&stack_mutex);
        return (value_t)0;
    }
    value_t data = (*top)->data;
    StackNode *temp = *top;
    *top = (*top)->next;
    free(temp);
    pthread_mutex_unlock(&stack_mutex);
    return data;
}

int is_empty(StackNode *top) {
    return top == NULL;
}