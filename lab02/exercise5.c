#include <stdio.h>
#include <string.h>
int tribonacci(int n) {
    int t0,t1,t2,tn;
    t0 = 0;
    t1 = 1;
    t2 = 1;
    tn = 0;
    if (n==0) {
        return 0;
    }
    if ((n<=2) && (n>=1)) {
        return 1;
    }
    else {
        for (int i=3;i<=n;i++) {
            tn = t0+t1+t2;
            t0 = t1;
            t1 = t2;
            t2 = tn;
        }
    }
    return tn;

}

int main() {
    printf("%d\n",tribonacci(4));
    printf("%d",tribonacci(36));
}
