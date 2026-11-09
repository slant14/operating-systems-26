#include <stdio.h>

int size = 3;

struct Node {
    int value;
    struct Node* next;
    struct Node* prev;
};

void insert_node(int value, struct Node arr[], int index) {
    arr[index].value = value;
    if (index == 0) {
        arr[index].prev = NULL;
        arr[index].next = NULL;
    }
    if (index > 0) {
        arr[index-1].next = &arr[index];
        arr[index].prev = &arr[index-1];
    }
    if (index == size - 1) {
        arr[index].next = NULL;
        arr[index].prev = &arr[index-1];
    }
}

void delete_node(struct Node arr[], int index) {
    if (index == size - 1) {
        arr[index-1].next = NULL;
    }
    else if (index == 0) {
        arr[index+1].prev = NULL;
    }
    else {
        arr[index-1].next = &arr[index+1];
        arr[index+1].prev = &arr[index-1];
    }
}

void print_list(struct Node arr[]) {
    struct Node* elem;
    for (int i=0;i<size;i++) {
        if (arr[i].prev == NULL) {
            elem = &arr[i];
        }
    }
    printf("%d ",elem->value);
    while (elem->next != NULL) {
        printf("%d ", (elem->next)->value);
        elem = elem->next;
   }
}

int main() {
    struct Node arr[3];
    int val;
    for (int i = 0; i < size; i++) {
        scanf("%d",&val);
        insert_node(val,arr,i);
    }

    delete_node(arr,1);
    print_list(arr);
}
