#include <stdio.h>
#include <math.h>

struct Point {
    double x;
    double y;
};

double distance(struct Point a, struct Point b)
{
    double dx = b.x - a.x;
    double dy = b.y - a.y;
    return sqrt(dx * dx + dy * dy);
}

double area(struct Point A, struct Point B, struct Point C)
{
    return 0.5 * fabs(A.x * B.y - B.x * A.y + B.x * C.y - C.x * B.y + C.x * A.y - A.x * C.y);
}

int main(void)
{

    struct Point A = {2.5, 6.0};
    struct Point B = {1.0, 2.2};
    struct Point C = {10.0, 6.0};

    double dAB = distance(A, B);
    printf("Distance AB: %.4f\n", dAB);

    double s = area(A, B, C);
    printf("Area ABC: %.4f\n", s);

    return 0;
}