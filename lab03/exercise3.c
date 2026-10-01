#include <stdio.h>

int size = 3;

struct Node {
    int value;
    struct Node* next;
};

void insert_node(int value, struct Node arr[], int index) {
    arr[index].value = value;
    if (index > 0) {
        arr[index-1].next = &arr[index];
    }
    if (index == size - 1) {
        arr[index].next = NULL;
    }
}

void delete_node(struct Node arr[], int index) {
    if (index == size - 1) {
        arr[index-1].next = NULL;
    }
    else if (index == 0) {
        arr[0] = arr[index+1];
    }
    else {
        arr[index-1].next = &arr[index+1];
    }
}

void print_list(struct Node arr[]) {
    struct Node* elem = &arr[0];
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
