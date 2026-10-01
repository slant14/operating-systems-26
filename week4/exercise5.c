#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
    struct Node* previous;
};

typedef struct doubly_linked_list {
    struct Node* head;
    struct Node* tail;
} doubly_linked_list;

void print_list(doubly_linked_list* list) {
    struct Node* current = list->head;
    if (current == NULL) {
        printf("\n");
        return;
    }
    while (current->next != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("%d", current->data);
    printf("\n");
}

void print_list_reverse(doubly_linked_list* list) {
    struct Node* current = list->tail;
    if (current == NULL) {
        printf("\n");
        return;
    }
    while (current->previous != NULL) {
        printf("%d ", current->data);
        current = current->previous;
    }
    printf("%d", current->data);
    printf("\n");
}

void insert_node(doubly_linked_list* list, struct Node* existing_element, int new_element) {
    struct Node* new_node = malloc(sizeof(struct Node));
    new_node->data = new_element;
    new_node->next = NULL;
    new_node->previous = NULL;
    if (list->head == NULL) {
        list->head = new_node;
        list->tail = new_node;
        return;
    }
    if (existing_element == NULL) {
        new_node->previous = list->tail;
        list->tail->next = new_node;
        list->tail = new_node;
    }
    else {
        struct Node* current = list->head;
        while (current != NULL && current != existing_element) {
            current = current->next;
        }
        if (current == NULL) {
            free(new_node);
            return;
        }
        struct Node* tmp = current->next;
        new_node->previous = current;
        new_node->next = tmp;
        current->next = new_node;
        if (tmp != NULL) {
            tmp->previous = new_node;
        }
        else {
            list->tail = new_node;
        }
    }
}

void delete_node(doubly_linked_list* list, int to_delete, int start_from_head) {
    struct Node* current;
    if (start_from_head == 1) {
        current = list->head;
        while (current != NULL && current->data != to_delete) {
            current = current->next;
        }
    }
    else {
        current = list->tail;
        while (current != NULL && current->data != to_delete) {
            current = current->previous;
        }
    }
    if (current == NULL) {
        return;
    }
    if (current->previous != NULL) {
        current->previous->next = current->next;
    }
    else {
        list->head = current->next;
    }
    if (current->next != NULL) {
        current->next->previous = current->previous;
    }
    else {
        list->tail = current->previous;
    }
    free(current);
}

int main() {
    doubly_linked_list list = {NULL, NULL};
    insert_node(&list, NULL, 5);
    insert_node(&list, NULL, 4);
    insert_node(&list, NULL, 52);
    insert_node(&list, NULL, 19);
    print_list(&list);
    print_list_reverse(&list);
    delete_node(&list, 4, 1);
    print_list(&list);
    insert_node(&list, list.head, 7);
    print_list(&list);
    delete_node(&list, 19, 0);
    print_list(&list);
    print_list_reverse(&list);
    return 0;
}