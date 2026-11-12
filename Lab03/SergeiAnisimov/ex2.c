/* Lab 03 Sergei Anisimov CSE-04 */
#include <math.h>
#include <stdio.h>

typedef struct Point {
    double x;
    double y;
} Point;

double distance(Point a, Point b)
{
    return hypot(a.x - b.x, a.y - b.y);
}

double area(Point a, Point b, Point c)
{
    return 0.5 * fabs(a.x * (b.y - c.y) +
                      b.x * (c.y - a.y) +
                      c.x * (a.y - b.y));
}

int main(void)
{
    Point a = {2.5, 6};
    Point b = {1, 2.2};
    Point c = {10, 6};
    printf("Distance AB: %.6f\n", distance(a, b));
    printf("Area ABC: %.6f\n", area(a, b, c));
    return 0;
}
