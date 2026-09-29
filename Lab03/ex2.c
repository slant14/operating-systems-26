#include<stdio.h>
#include<stdlib.h>
#include<math.h>

struct Point
{
    double x;
    double y;
} A, B, C;

double distance (double x1, double x2, double y1, double y2) {
    return sqrtf((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

double area (double x1, double x2, double x3, double y1, double y2, double y3) {
    return 0.5 * fabs(x1 * y2 - x2* y1 + x2 * y3  - x3 * y2 + x3 * y1 - y3 * x1);
}


int main() {

A.x = 2.5;
A.y = 6;
B.x = 1;
B.y = 2.2;
C.x = 10;
C.y = 6;
double dist = distance (A.x, B.x, A.y, B.y);
double a = area (A.x, B.x, C.x, A.y, B.y, C.y);
printf("%f\n", dist);
printf("%f\n", a);
return 0;
}