#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

typedef struct linked_list {
    struct Node* head;
} linked_list;

void print_list(linked_list* list) {
    struct Node* current = list->head;
    if (current == NULL) {
        return;
    }
    while (current->next != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("%d", current->data);
    printf("\n");
}

void insert_node(linked_list* list, struct Node* existing_element, int new_element) {
    if (list->head == NULL) {
        struct Node* first = malloc(sizeof(struct Node));
        list->head = first;
        first->data = new_element;
        first->next = NULL;
        return;
    }
    struct Node* current = list->head;
    if (existing_element == NULL) {
        struct Node* new_node = malloc(sizeof(struct Node));
        while (current->next != NULL) {
            current = current->next;
        }
        new_node->data = new_element;
        new_node->next = NULL;
        current->next = new_node;
    }
    else {
        while (current != existing_element) {
            current = current->next;
        }
        struct Node* tmp = current->next;
        struct Node* new_node = malloc(sizeof(struct Node));
        new_node->data = new_element;
        current->next = new_node;
        new_node->next = tmp;
    }
}

void delete_node(linked_list* list, int to_delete) {
    struct Node* current = list->head;
    if (current == NULL) {
        return;
    }
    if (current->data == to_delete) {
        list->head = current->next;
        return;
    }
    while (current->next != NULL && current->next->data != to_delete) {
        current = current->next;
    }
    if (current->next != NULL) {
        current->next = current->next->next;
    }
}

int main() {
    linked_list list = {NULL};
    insert_node(&list, NULL, 5);
    insert_node(&list, NULL, 4);
    insert_node(&list, NULL, 52);
    insert_node(&list, NULL, 19);
    print_list(&list);
    delete_node(&list, 4);
    print_list(&list);
    return 0;
}