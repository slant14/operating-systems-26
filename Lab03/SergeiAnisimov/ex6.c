/* Lab 03 Sergei Anisimov CSE-04 */
#include <stdio.h>

static void swap(int *a, int *b)
{
    int value = *a;
    *a = *b;
    *b = value;
}

static size_t partition(int values[], size_t count)
{
    size_t last = count - 1;
    swap(&values[count / 2], &values[last]);
    int pivot = values[last];
    size_t position = 0;

    for (size_t i = 0; i < last; ++i) {
        if (values[i] < pivot)
            swap(&values[position++], &values[i]);
    }
    swap(&values[position], &values[last]);
    return position;
}

void quicksort(int values[], size_t count)
{
    while (count > 1) {
        size_t pivot = partition(values, count);
        size_t right_count = count - pivot - 1;

        /* Recurse on the smaller partition to bound stack usage. */
        if (pivot < right_count) {
            quicksort(values, pivot);
            values += pivot + 1;
            count = right_count;
        } else {
            quicksort(values + pivot + 1, right_count);
            count = pivot;
        }
    }
}

int main(void)
{
    int values[] = {8, -4, 9, 1, 0, 5, 8, -4, 2};
    size_t count = sizeof values / sizeof *values;

    quicksort(values, count);
    for (size_t i = 0; i < count; ++i)
        printf("%d%c", values[i], i + 1 == count ? '\n' : ' ');
    return 0;
}
