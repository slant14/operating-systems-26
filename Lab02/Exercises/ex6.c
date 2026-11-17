#include <stdio.h>
#include <stdlib.h>   

void print_right_triangle(size_t dim) {
    for (size_t i = 1; i <= dim; ++i) {
        for (size_t j = 1; j <= i; ++j)
            putchar('*');
        putchar('\n');
    }
}

void print_triangle(size_t dim) {
    for (size_t i = 1; i <= dim / 2; ++i) {
        for (size_t j = 1; j <= i; ++j)
            putchar('*');
        putchar('\n');
    }
    for (size_t i = dim / 2 + 1; i > 0; --i) {
        for (size_t j = 1; j <= i; ++j)
            putchar('*');
        putchar('\n');
    }
}

void print_rectangle(size_t height, size_t width) {
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j)
            putchar('*');
        putchar('\n');
    }
}

void show_menu(void) {
    printf("\n=== Noice Shape Drawer ===\n");
    printf("1. Right triangle\n");
    printf("2. Diamond-like triangle\n");
    printf("3. Rectangle\n");
    printf("4. Quit\n");
    printf("Choose an option: ");
}

int main(void) {
    size_t choice = 0;

    do {
        show_menu();

        if (scanf("%zu", &choice) != 1) {
            fprintf(stderr, "Invalid input. Exiting.\n");
            return EXIT_FAILURE;
        }

        if (choice == 1) {
            size_t dim;
            printf("Enter size: ");
            if (scanf("%zu", &dim) == 1)
                print_right_triangle(dim);
        } else if (choice == 2) {
            size_t dim;
            printf("Enter size: ");
            if (scanf("%zu", &dim) == 1)
                print_triangle(dim);
        } else if (choice == 3) {
            size_t height, width;
            printf("Enter height: ");
            if (scanf("%zu", &height) != 1) continue;
            printf("Enter width: ");
            if (scanf("%zu", &width) != 1) continue;
            print_rectangle(height, width);
        } else if (choice != 4) {
            printf("Unknown option. Try again.\n");
        }

    } while (choice != 4);

    printf("Goodbye! Have a noice day!\n");
    return EXIT_SUCCESS;
}

