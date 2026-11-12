/* Lab 02 Sergei Anisimov CSE-04 */
#include <stdio.h>

static void stars(int count)
{
    while (count-- > 0)
        putchar('*');
}

int main(void)
{
    for (int row = 1; row <= 7; ++row) {
        int middle = row <= 4 ? row : 8 - row;

        stars(row);
        printf("%*s", 10 - row, "");
        stars(middle);
        printf("%*s", 10 - middle, "");
        stars(7);
        putchar('\n');
    }
    return 0;
}
